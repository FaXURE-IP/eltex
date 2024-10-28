#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/stream_serv"
#define BUFFER_SIZE 1024

void run_server()
{
    struct sockaddr_un serv_addr, client_addr;

    socklen_t len = sizeof(struct sockaddr_un);
    int fd, client_fd, n;

    char buffer[BUFFER_SIZE], buffer_copy[BUFFER_SIZE];

    fd = socket(AF_LOCAL, SOCK_STREAM, 0);

    if (fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_un));
    serv_addr.sun_family = AF_LOCAL;

    strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path) - 1);
    unlink(SOCKET_PATH);

    if (bind(fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(fd, 1) == -1)
    {
        perror("listen");
        close(fd);
        exit(EXIT_FAILURE);
    }

    client_fd = accept(fd, (struct sockaddr *)&client_addr, &len);

    if (client_fd == -1)
    {
        perror("accept");
        close(fd);
        exit(EXIT_FAILURE);
    }

    n = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (n == -1)
    {
        perror("recv");
        close(client_fd);
        close(fd);
        exit(EXIT_FAILURE);
    }

    strncpy(buffer_copy, buffer, 4);

    if (n == 3) {
        buffer[2] = '!';
        buffer[3] = '\0';
    }

    if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
        perror("send");
        close(fd);
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server \nSocket path: %s\nClient send: %s\nclient will get : %s\n", SOCKET_PATH, buffer, buffer_copy);

    close(fd);
    close(client_fd);
    unlink(SOCKET_PATH);
}


int main(){

    run_server();
    return 0;
}