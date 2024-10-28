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
#include <sys/ipc.h>
#include <sys/msg.h>

#define BASE_PORT 7777
#define BUFFER_SIZE 1024
#define SERVER_POOL_SIZE 5
#define QUEUE_KEY 1234

struct socket_info
{
    int port;
    pthread_t thread_id;
};

struct msg_buffer
{
    long msg_type;
    int client_fd;
};

struct socket_info* server_pool;
pthread_mutex_t pool_mutex = PTHREAD_MUTEX_INITIALIZER;

void* server_thread(void* arg) {
    struct socket_info* info = (struct socket_info*)arg;
    char buffer[BUFFER_SIZE] = {0}, response[BUFFER_SIZE];
    int msgid = msgget(QUEUE_KEY, 0666 | IPC_CREAT);

    sprintf(response, "Hello from server on port %d!", info->port);
    printf("Server listening on port %d\n", info->port);

    while (1) {
        struct msg_buffer message;
        if (msgrcv(msgid, &message, sizeof(message.client_fd), 0, 0) == -1) {
            perror("msgrcv");
            continue;
        }

        handle_client_request(info, message.client_fd, buffer, response);
    }

    pthread_exit(NULL);
}

void init_server_pool() {
    server_pool = (struct socket_info*)malloc(SERVER_POOL_SIZE * sizeof(struct socket_info));
    if (server_pool == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SERVER_POOL_SIZE; i++) {
        server_pool[i].port = BASE_PORT + i + 1;

        if (pthread_create(&server_pool[i].thread_id, NULL, server_thread, &server_pool[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }

        printf("Created server thread for port %d\n", server_pool[i].port);
    }
}

int init_listener_socket() {
    int listener_fd;
    struct sockaddr_in listener_addr;

    if ((listener_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    listener_addr.sin_family = AF_INET;
    listener_addr.sin_addr.s_addr = INADDR_ANY;
    listener_addr.sin_port = htons(BASE_PORT);

    if (bind(listener_fd, (struct sockaddr*)&listener_addr, sizeof(listener_addr)) < 0) {
        perror("bind");
        close(listener_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(listener_fd, 5) < 0) {
        perror("listen");
        close(listener_fd);
        exit(EXIT_FAILURE);
    }

    printf("Main server listening on port %d\n", BASE_PORT);

    return listener_fd;
}

void enqueue_client_request(int client_fd, int msgid) {
    struct msg_buffer message;
    message.msg_type = 1;
    message.client_fd = client_fd;

    if (msgsnd(msgid, &message, sizeof(message.client_fd), 0) == -1) {
        perror("msgsnd");
    }
}

void listener_main_loop(int listener_fd, int msgid) {
    struct sockaddr_in listener_addr;
    int addrlen = sizeof(listener_addr);

    while (1) {
        int new_socket = accept(listener_fd, (struct sockaddr*)&listener_addr, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        enqueue_client_request(new_socket, msgid);
    }
}

void handle_client_request(struct socket_info* info, int client_fd, char* buffer, char* response) {
    ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("read");
    } else {
        printf("Received from client on port %d: %s\n", info->port, buffer);
        if (send(client_fd, response, strlen(response), 0) == -1) {
            perror("send");
            exit(EXIT_FAILURE);
        }
        printf("Sent to client on port %d: %s\n", info->port, response);
    }

    close(client_fd);
}

void sigint_handler(int sig) {
    printf("Exiting...\n");
    for (int i = 0; i < SERVER_POOL_SIZE; i++) {
        pthread_cancel(server_pool[i].thread_id);
    }
    free(server_pool);
    exit(EXIT_SUCCESS);
}

void run_server() {
    signal(SIGINT, sigint_handler);

    init_server_pool();

    int listener_fd = init_listener_socket();
    int msgid = msgget(QUEUE_KEY, 0666 | IPC_CREAT);

    listener_main_loop(listener_fd, msgid);

    close(listener_fd);
}
int main()
{
    run_server();
    return 0;
}