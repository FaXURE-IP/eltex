#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define BUFFER_SIZE 1024

void run_client()
{

    int sock_fd;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(addr);

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int recv_len = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&addr, &addr_len);
        if (recv_len < 0)
        {
            perror("recvfrom");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }
        buffer[recv_len] = '\0';
        printf("Received: %s\n", buffer);
    }

    close(sock_fd);
}

int main()
{
    run_client();
    return 0;
}