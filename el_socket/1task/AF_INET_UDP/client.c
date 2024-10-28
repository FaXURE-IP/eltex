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

void run_client(){

    int sock = 0;
    struct sockaddr_in serv_addr, client_addr;
    char buffer[BUFFER_SIZE];
    char *message = "Hi?";
    socklen_t len = sizeof(client_addr);

    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = 0;
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    if(bind(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0){
        perror("bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
        perror("inet pton");
        close(sock);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_sent = sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(bytes_sent < 0){
        perror("sendto");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if(getsockname(sock, (struct sockaddr *)&client_addr, &len) < 0){
        perror("getsockname");
        close(sock);
        exit(EXIT_FAILURE);
    }

    char clinet_ip[INET_ADDRSTRLEN];
    if(inet_ntop(AF_INET, &client_addr.sin_addr, clinet_ip, INET_ADDRSTRLEN) == NULL){
        perror("inet_ntop");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Client IP: %s:%d\n", clinet_ip, ntohs(client_addr.sin_port));
    printf("Message sent: %s\n", message);

    ssize_t bytes_recieved = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &len);
    if(bytes_recieved < 0){
        perror("recvfrom");
        close(sock);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_recieved] = '\0';
    printf("Message from server: %s\n", buffer);
    close(sock);
}

int main(){
    run_client();
    return 0;
}