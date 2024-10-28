#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
#define OFFSET 28

void print_buffer(char *buffer, int len, int offset)
{

    for (int i = 0; i < len; i++)
    {
        printf("%c", buffer[i]);
    }

    printf("\n");
}

void run_server()
{

    int server_fd;
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

    if (server_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        ssize_t bytes_read = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (bytes_read < 0)
        {
            perror("recvfrom");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        if (buffer[OFFSET] == 'H')
        {
            print_buffer(buffer, bytes_read, OFFSET);
        }
    }
    close(server_fd);
}

int main() {
    run_server();
    return 0;
}