#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH_SERVER "/tmp/stream_server"
#define SOCKET_PATH_CLIENT "/tmp/stream_client"
#define BUFFER_SIZE 1024

void run_server()
{
    struct sockaddr_un server_addr, client_addr;

    socklen_t len = sizeof(struct sockaddr_un);

    int fd, n;

    char buffer[BUFFER_SIZE], buffer_copy[BUFFER_SIZE];

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    if (fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_LOCAL;
    strncpy(server_addr.sun_path, SOCKET_PATH_SERVER, sizeof(server_addr.sun_path) - 1);

    unlink(SOCKET_PATH_SERVER);

    if (bind(fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    n = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &len);
    if (n == -1)
    {
        perror("recvfrom");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (n == 3)
    {
        buffer[3] = '\0';
    }

    strcpy(buffer_copy, buffer);

    buffer[2] = '!';

    if (sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, len) == -1)
    {
        perror("sendto");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Server \nSocket path: %s\nClient socket path: %s\nClient send: %s\nClient will get : %s\n", SOCKET_PATH_SERVER, SOCKET_PATH_CLIENT, buffer, buffer_copy);

    close(fd);
    unlink(SOCKET_PATH_SERVER);
}

int main(){
    run_server();
    return 0;

}