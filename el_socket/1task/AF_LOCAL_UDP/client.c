#include <netinet/in.h>
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

void run_client()
{

    struct sockaddr_un serv_addr, client_addr;

    int fd;

    char buffer[BUFFER_SIZE];

    char *message = "Hi!";

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    if (fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_un));
    serv_addr.sun_family = AF_LOCAL;
    strncpy(serv_addr.sun_path, SOCKET_PATH_SERVER, sizeof(serv_addr.sun_path) - 1);

    memset(&client_addr, 0, sizeof(struct sockaddr_un));
    client_addr.sun_family = AF_LOCAL;
    strncpy(client_addr.sun_path, SOCKET_PATH_CLIENT, sizeof(client_addr.sun_path) - 1);

    unlink(SOCKET_PATH_CLIENT);

    if (bind(fd, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (sendto(fd, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("sendto");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ssize_t n = recvfrom(fd, buffer, BUFFER_SIZE, 0, NULL, NULL);

    if (n == -1)
    {
        perror("recvfrom");
        close(fd);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';

    printf("Client\nSocket server path: %s\nSocket client path: %s\nServer will get: %s\nServer sent: %s\n", SOCKET_PATH_SERVER, SOCKET_PATH_CLIENT, message, buffer);

    close(fd);
    unlink(SOCKET_PATH_CLIENT);
}

int main()
{
    run_client();
    return 0;
}