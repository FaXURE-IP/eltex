#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/stream_serv"
#define BUFFER_SIZE 1024


void run_client() {
    struct sockaddr_un serv_addr;

    char buffer[BUFFER_SIZE];
    char *message = "Hi!";
    int fd;

    fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    memset(&serv_addr, 0, sizeof(struct sockaddr_un));
    serv_addr.sun_family = AF_LOCAL;
    strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        close(fd);
        exit(EXIT_FAILURE); 
    }

    if(send(fd, message, strlen(message), 0) == -1) {
        perror("send");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ssize_t n = recv(fd, buffer, sizeof(buffer), 0);

    if(n == -1) {
        perror("recv");
        close(fd);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';

    printf("Client\nScoekt path: %s\nServer will get message: %s\nServer sent message: %s\n", SOCKET_PATH, message, buffer);

    close(fd);
}

int main() {
    run_client();
    return 0;
}
