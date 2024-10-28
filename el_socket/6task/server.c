#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 64

void run_server()
{
    int server_fd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in address, client_address;
    socklen_t addrlen = sizeof(client_address);
    char *response = "Hi!";

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("Server listening from port %d\n", PORT);

    ssize_t bytes_read = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &addrlen);
    if (bytes_read < 0)
    {
        perror("recvfrom");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client's port %d\n", ntohs(client_address.sin_port));
    printf("Client response: %s\n", buffer);

    ssize_t bytes_sent = sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_address, addrlen);
    if (bytes_sent < 0)
    {
        perror("sendto");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Send to client: %s\n", response);

    close(server_fd);
}

int main()
{
    run_server();
    return 0;
}