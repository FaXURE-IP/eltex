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
#define BUFFER_SIZE 1024

void run_client() {
    int sock = 0;
    struct sockaddr_in serv_addr, client_addr;
    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hi?";
    socklen_t addr_len = sizeof(client_addr);
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
    if (getsockname(sock, (struct sockaddr *)&client_addr, &addr_len) == -1) {
        perror("getsockname");
        close(sock);
        exit(EXIT_FAILURE);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("I'm client\nClient started on %s:%d\n", client_ip, ntohs(client_addr.sin_port));
    
    ssize_t bytes_sent = send(sock, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("send");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Sent to server: %s\n", message);
    
    ssize_t bytes_read = read(sock, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("read");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Received from server: %s\n", buffer);
    
    close(sock);
}

int main() {
    run_client();
    return 0;
}