#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 7777
#define BUFFER_SIZE 1024

void run_client()
{
    int sock = 0;

    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char *message = "Hi!";

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(sock, buffer, BUFFER_SIZE);
    if (bytes_read < 0)
    {
        perror("read");
        close(sock);
        exit(EXIT_FAILURE);
    }
    int new_port = atoi(buffer);

    printf("New port: %d\n", new_port);
    close(sock);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_port = htons(new_port);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(sock, message, strlen(message));
    if (bytes_written < 0)
    {
        perror("write");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Message sent to new server: %s\n", message);

    bytes_read = read(sock, buffer, BUFFER_SIZE);
    if (bytes_read < 0)
    {
        perror("read");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Message from new server: %s\n", buffer);
    close(sock);
}

int main()
{
    run_client();
    return 0;
}
