#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAIN_SERVER_PORT 7777
#define BUFFER_SIZE 1024

void connect_server(int port)
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
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("inet pton");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_send = send(sock, message, BUFFER_SIZE, 0);

    if (bytes_send < 0)
    {
        perror("send");
        close(sock);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(sock, buffer, BUFFER_SIZE);

    if (bytes_read < 0)
    {
        perror("read");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Recieved from server on port %d: %s\n", port, buffer);

    close(sock);
}

void run_client()
{
    int sock = 0;

    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(MAIN_SERVER_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("inet pton");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        close(sock);
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

    printf("Recieved new server port: %d\n", new_port);

    close(sock);

    connect_server(new_port);
}
int main()
{
    run_client();
    return 0;
}