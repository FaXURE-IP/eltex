#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <ifaddrs.h>
#include <net/if.h>

#define SERVER_PORT 12345
#define CLIENT_PORT 9999
#define BUFFER_SIZE 1500
#define SERVER_IP "192.168.0.13/24"
#define CLIENT_IP "192.168.0.17"
#define SERVER_MAC {0xf4,0xa4,0x75,0x1f,0x98,0x11}
#define CLIENT_MAC {0x38,0xd5,0x7a,0xe7,0xa8,0x21}
#define CLIENT_IF "wlp2s0"
#define OFFSET 42

void run_client() {
    int fd;
    char buffer[BUFFER_SIZE];
    char message[] = "Hi?";
    struct sockaddr_ll addr = {0};
    struct ethhdr eth_header;
    struct iphdr ip_header;
    struct udphdr udp_header;
    unsigned char source_mac[6] = CLIENT_MAC, destination_mac[6] = SERVER_MAC;

    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Client started on interface %s\n", CLIENT_IF);

    fill_ethernet_header(&eth_header, source_mac, destination_mac);
    fill_udp_header(&udp_header, sizeof(message));
    fill_ip_header(&ip_header, sizeof(message));
    memcpy(buffer, &eth_header, sizeof(eth_header));
    memcpy(buffer + sizeof(eth_header), &ip_header, sizeof(ip_header));
    memcpy(buffer + sizeof(eth_header) + sizeof(ip_header), &udp_header, sizeof(udp_header));
    memcpy(buffer + sizeof(eth_header) + sizeof(ip_header) + sizeof(udp_header), message, sizeof(message));


    addr.sll_ifindex = if_nametoindex(CLIENT_IF);
    addr.sll_halen = ETH_ALEN;
    memcpy(addr.sll_addr, destination_mac, 6);

    sendto(fd, buffer, sizeof(eth_header) + sizeof(ip_header) + sizeof(udp_header) + sizeof(message), 0, (struct sockaddr*)&addr, sizeof(addr));

    printf("Sent \"%s\" to server on port %d\n", message, SERVER_PORT);

    while (1) {
        ssize_t bytes_received = recvfrom(fd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (bytes_received == -1) {
            perror("recvfrom");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (buffer[OFFSET + 2] == '!') {
            printf("Received from server: ");
            print_buffer(buffer, (int)bytes_received, OFFSET);
            break;
        }
    }

    close(fd);
}

void fill_ethernet_header(struct ethhdr* eth_header, unsigned char* source_mac, unsigned char* destination_mac) {
    memcpy(eth_header->h_dest, destination_mac, 6);  
    memcpy(eth_header->h_source, source_mac, 6); 
    eth_header->h_proto = htons(ETH_P_IP);
}

void fill_udp_header(struct udphdr* udp_header, int msg_size) {
    udp_header->uh_sport = htons(CLIENT_PORT);
    udp_header->uh_dport = htons(SERVER_PORT);
    udp_header->len = htons(sizeof(struct udphdr) + msg_size);
    udp_header->check = 0;
}

void fill_ip_header(struct iphdr* ip_header, int msg_size) {
    ip_header->ihl = 5; 
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + msg_size);
    ip_header->id = 2;
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = IPPROTO_UDP;
    ip_header->saddr = inet_addr(CLIENT_IP);
    ip_header->daddr = inet_addr(SERVER_IP);
    ip_header->check = 0;
    ip_header->check = checksum((uint16_t *) ip_header, ip_header->ihl * 4);
}

int checksum(uint16_t* buffer, int len) {
    unsigned long csum = 0;

    while (len > 1) {
        csum += *buffer++;
        len -= 2;
    }

    if (len == 1) {
        csum += *(uint8_t*) buffer;
    }

    csum = (csum >> 16) + (csum & 0xFFFF);
    csum += (csum >> 16);

    return ~csum;
}

void print_buffer(char* buffer, int len, int offset) {
    for (int i = offset; i < len; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}

int main() {
    run_client();
    return 0;
}