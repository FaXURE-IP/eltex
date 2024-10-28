#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888
#define MULTICAST_IP "239.0.0.1"
#define BUFFER_SIZE 1024

void run_client()
{
    int sock_fd;
    struct sockaddr_in addr;
    struct ip_mreq mreq;
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

    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt");
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
        printf("Received from server: %s\n", buffer);
    }

    close(sock_fd);
}

int main()
{
    run_client();
    return 0;
}