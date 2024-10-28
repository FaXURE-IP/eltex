#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define SERVER_PORT 12345
#define CLIENT_PORT 9999
#define BUFFER_SIZE 64
#define OFFSET 28

/**
 * Fill in the fields of a udphdr struct.
 *
 * @param udp_header The udphdr struct to fill in.
 * @param msg_size The size of the UDP packet.
 */
void fill_udp_header(struct udphdr* udp_header, int msg_size) {
    udp_header->uh_sport = htons(CLIENT_PORT);
    udp_header->uh_dport = htons(SERVER_PORT);
    udp_header->len = htons(sizeof(udp_header) + msg_size);
    udp_header->check = 0;
}

/**
 * Fill in the fields of an iphdr struct given the size of the UDP packet.
 *
 * @param ip_header The iphdr struct to fill in.
 * @param msg_size The size of the UDP packet.
 */
void fill_ip_header(struct iphdr *ip_header, int msg_size)
{
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = htons(sizeof(ip_header) + sizeof(struct udphdr) + msg_size);
    ip_header->id = 0;
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = IPPROTO_UDP;
    ip_header->check = 0;
    ip_header->saddr = htonl(INADDR_LOOPBACK);
    ip_header->daddr = htonl(INADDR_LOOPBACK);
}

void print_buffer(char *buffer, int len, int offset)
{
    for (int i = offset; i < len; i++)
    {
        printf("%c", buffer[i]);
    }
    printf("\n");
}

void run_client()
{
    int client_fd;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE] = "Hi?";
    struct sockaddr_in server_addr;
    struct udphdr udp_header;
    struct iphdr ip_header;

    client_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

    if (client_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    int one = 1;
    if (setsockopt(client_fd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) { 
        perror("setsockopt");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client started on port %d\n", CLIENT_PORT);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    fill_udp_header(&udp_header, strlen(message));
    fill_ip_header(&ip_header, strlen(message));

    memcpy(buffer, &ip_header, sizeof(ip_header));
    memcpy(buffer + sizeof(ip_header), &udp_header, sizeof(udp_header));
    memcpy(buffer + sizeof(ip_header) + sizeof(udp_header), message, strlen(message));

    if (sendto(client_fd, buffer, strlen(message) + sizeof(ip_header) + sizeof(udp_header), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("sendto");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Sent to server: %s on port %d\n", message, SERVER_PORT);

    while (1)
    {
        ssize_t bytes_recieved = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_recieved < 0)
        {
            perror("recv");
            close(client_fd);
            exit(EXIT_FAILURE);
        }
        if (buffer[30] == '!')
        {
            printf("Received from server");
            print_buffer(buffer, bytes_recieved, OFFSET);
            break;
        }
    }
    close(client_fd);
}

int main()
{
    run_client();
    return 0;
}