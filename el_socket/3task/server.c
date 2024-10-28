#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define MULTICAST_IP "239.0.0.1"
#define MESSAGE "Hi!"
#define TIMEOUT 3

void run_server()
{
    int sock_fd;

    struct sockaddr_in addr;

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(MULTICAST_IP);

    while (1) {
        if (sendto(sock_fd, MESSAGE, strlen(MESSAGE), 0, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("sendto");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }

        printf("Sent multicast: %s\n", MESSAGE);
        sleep(TIMEOUT);
    }

    close(sock_fd);
}

int main()
{
    run_server();
    return 0;
}