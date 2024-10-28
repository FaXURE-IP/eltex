#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define BASE_PORT 7777
#define BUFFER_SIZE 1024
#define NEW_PORT (BASE_PORT + rand() % 10000 + 1)
int server_fd;

void *create_srever(void *arg)
{

    int new_port = *((int *)arg);
    free(arg);

    int server_fd, new_socket;

    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE], response[BUFFER_SIZE];
    sprintf(response, "Hi! I am server %d", new_port);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(new_port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening from port %d\n", new_port);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE);
    if (bytes_read < 0)
    {
        perror("read");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Received: %s\n", buffer);

    ssize_t bytes_write = write(new_socket, response, BUFFER_SIZE);

    if (bytes_write < 0)
    {
        perror("write");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Sent: %s\n", response);

    close(new_socket);
    close(server_fd);

    return NULL;
}

void sigint_handler(int sig)
{
    printf("Exiting...\n");
    close(server_fd);
    exit(EXIT_SUCCESS);
}

void run_server()
{
    signal(SIGINT, sigint_handler);

    server_fd = 1;
    int new_socket;
    struct sockaddr_in address;

    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(BASE_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening from port %d\n", BASE_PORT);

    while (1)
    {

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        int new_port = NEW_PORT;
        printf("New server on new port: %d\n", new_port);

        int *port_ptr = malloc(sizeof(int));

        if (port_ptr == NULL)
        {
            perror("malloc");
            close(new_socket);
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        *port_ptr = new_port;

        pthread_t thread;
        if (pthread_create(&thread, NULL, create_srever, port_ptr) < 0)
        {
            perror("pthread_create");
            close(new_socket);
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        sprintf(buffer, "%d", new_port);
        if (send(new_socket, buffer, strlen(buffer), 0) < 0)
        {
            perror("send");
            close(new_socket);
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        close(new_socket);
    }
    close(server_fd);
}

int main()
{
    run_server();
    return 0;
}
