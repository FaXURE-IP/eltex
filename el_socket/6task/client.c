#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/udp.h>

#define SERVER_PORT 12345
#define CLIENT_PORT 9999 // This line defines CLIENT_PORT
#define BUFFER_SIZE 64
#define OFFSET 28

void fill_udp_header(struct udphdr *udp_header, int msg_size)
{
    udp_header->uh_sport = htons(CLIENT_PORT);
    udp_header->uh_dport = htons(SERVER_PORT);
    udp_header->len = htons(msg_size + OFFSET);
    udp_header->check = 0;
}

void print_buffer(char *buffer, int len, int offset)
{
    for (int i = offset; i < len; i++)
    {
        printf("%c", buffer[i]);
    }
    printf("\n");
}


void run_client(){
    int client_fd;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE] = "Hi?";
    struct sockaddr_in server_addr;
    struct udphdr udp_header;

    client_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(client_fd < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Client on port %d\n", CLIENT_PORT);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(INADDR_LOOPBACK);

    fill_udp_header(&udp_header, strlen(message));

    memcpy(buffer, &udp_header, OFFSET);
    memcpy(buffer + OFFSET, message, strlen(message));

    if(sendto(client_fd, buffer, strlen(message) + OFFSET, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("sendto");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Sent %s to server on port %d \n", message, SERVER_PORT);




    while(1){
        ssize_t bytes_recieved = recvfrom(client_fd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if(bytes_recieved < 0){
            perror("recvfrom");
            close(client_fd);
            exit(EXIT_FAILURE);
        }
        if(buffer[OFFSET] == '!'){
            printf("Recieved from server:");
            print_buffer(buffer, bytes_recieved, OFFSET);
        }
    }
    close(client_fd);
    
}

int main()
{
    run_client();
    return 0;
}