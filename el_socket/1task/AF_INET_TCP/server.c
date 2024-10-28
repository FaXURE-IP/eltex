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

void run_server()
{
    int server_fd, new_socket;

    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    char *message = "Hi!";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    printf("Server listening from port %d\n", PORT);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 1) < 0){
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Client started on %s:%d\n", client_ip, ntohs(address.sin_port));

    ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE);
    if (bytes_read == -1)
    {
        perror("read");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Received message from client: %s\n", buffer);


    ssize_t bytes_sent = send(new_socket, message, strlen(message), 0);
    if (bytes_sent == -1)
    {
        perror("send");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Sent message to client: %s\n", message);

    close(new_socket);
    close(server_fd);
}

int main(){
    run_server();
    return 0;
}