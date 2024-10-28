#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 12346
#define BUFFER_SIZE 1024


void run_client() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hi?";

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_sent = sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (bytes_sent == -1) {
        perror("sendto");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Sent to server: %s\n", message);

    ssize_t bytes_received = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (bytes_received == -1) {
        perror("recvfrom");
        close(sock);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("Received from server: %s\n", buffer);

    close(sock);
}
int main() {
    run_client();
    return 0;
}