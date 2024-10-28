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


void run_server(){

    int server_fd;

    struct sockaddr_in server_address, client_address;
    socklen_t addrlen = sizeof(server_address);
    char buffer[BUFFER_SIZE];
    char *message = "Hi!";

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening from port %d\n", PORT);

    ssize_t bytes_recieved = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &addrlen);
    if(bytes_recieved < 0){
        perror("recvfrom");
        close(server_fd);
        exit(EXIT_FAILURE);
    }


    char client_ip [INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Client connected from IP: %s:%d\n", client_ip, ntohs(client_address.sin_port));
    printf("Client message: %s\n", buffer);
    

    ssize_t bytes_send = sendto(server_fd, message, strlen(message), 0, (struct sockaddr *)&client_address, addrlen);
    if(bytes_send < 0){
        perror("sendto");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server sent message: %s\n", message);
    close(server_fd);
}

int main(){
    run_server();
    return 0;
}