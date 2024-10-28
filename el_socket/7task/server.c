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


void run_server() {
    int server_fd;
    struct sockaddr_in address, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "Hi!";

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    ssize_t bytes_received = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
    if (bytes_received == -1) {
        perror("recvfrom");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected from port %d\n", ntohs(client_addr.sin_port));
    printf("Received from client: %s\n", buffer);

    ssize_t bytes_sent = sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_addr_len);
    if (bytes_sent == -1) {
        perror("sendto");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Sent to client: %s\n", response);

    close(server_fd);
}

int main() {
    run_server();
    return 0;
}