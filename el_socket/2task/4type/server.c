#include <arpa/inet.h>
#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <malloc.h>
#include <errno.h>

#define PORT_TCP 12345
#define PORT_UDP 12346
#define BUFFER_SIZE 1024

int server_fd_udp, server_fd_tcp;
void sigint_handler(int sig) {
    printf("Exiting...\n");

    close(server_fd_tcp);
    close(server_fd_udp);
    
    exit(EXIT_SUCCESS);
}

void run_server_select() {
    signal(SIGINT, sigint_handler);

    fd_set readfds;
    int ready, nfds = ((PORT_TCP > PORT_UDP) ? PORT_TCP : PORT_UDP) + 1;


    struct sockaddr_in address_udp, address_tcp;

    init_udp_fd(&server_fd_udp, &address_udp);
    init_tcp_fd(&server_fd_tcp, &address_tcp);
    
    printf("SELECT\nServer listening on TCP port %d\n", PORT_TCP);
    printf("Server listening on UDP port %d\n", PORT_UDP);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd_tcp, &readfds);
        FD_SET(server_fd_udp, &readfds);

        ready = select(nfds, &readfds, NULL, NULL, NULL);

        if (ready == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(server_fd_tcp, &readfds)) {
            printf("TCP fd got a message\n");
            process_tcp(server_fd_tcp, address_tcp);
        }
        if (FD_ISSET(server_fd_udp, &readfds)) {
            printf("UDP fd got a message\n");
            process_udp(server_fd_udp);
        }
    }

    close(server_fd_tcp);
    close(server_fd_udp);
}

void run_server_poll() {
    int ready;
    struct pollfd* pollFD;
    pollFD = calloc(2, sizeof(struct pollfd));

    struct sockaddr_in address_udp, address_tcp;

    init_udp_fd(&server_fd_udp, &address_udp);
    init_tcp_fd(&server_fd_tcp, &address_tcp);
    
    printf("POLL\nServer listening on TCP port %d\n", PORT_TCP);
    printf("Server listening on UDP port %d\n", PORT_UDP);

    pollFD[0].fd = server_fd_tcp;
    pollFD[0].events = POLLIN;

    pollFD[1].fd = server_fd_udp;
    pollFD[1].events = POLLIN;


    while (1) {
        ready = poll(pollFD, 2, -1);

        if (ready == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }
        if (pollFD[0].revents & POLLIN) {
            printf("TCP fd got a message\n");
            process_tcp(server_fd_tcp, address_tcp);
        }
        if (pollFD[1].revents & POLLIN) {
            printf("UDP fd got a message\n");
            process_udp(server_fd_udp);
        }
    }

    free(pollFD);
    close(server_fd_tcp);
    close(server_fd_udp);
}

void run_server_epoll() {
    int ready, epfd;
    struct epoll_event ev;
    struct epoll_event evlist[2];
    struct sockaddr_in address_udp, address_tcp;

    init_udp_fd(&server_fd_udp, &address_udp);
    init_tcp_fd(&server_fd_tcp, &address_tcp);
    
    printf("EPOLL\nServer listening on TCP port %d\n", PORT_TCP);
    printf("Server listening on UDP port %d\n", PORT_UDP);

    epfd = epoll_create(2);
    if (epfd == -1) {
        perror("epoll");
        exit(EXIT_FAILURE);
    }

    ev.data.fd = server_fd_tcp;
    ev.events = EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd_tcp, &ev) == -1) {
        perror("epoll");
        exit(EXIT_FAILURE);
    }

    ev.data.fd = server_fd_udp;
    ev.events = EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd_udp, &ev) == -1) {
        perror("epoll");
        exit(EXIT_FAILURE);
    }

    while (1) {
        ready = epoll_wait(epfd, evlist, 2, -1);

        if (ready == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("epoll");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < ready; i++) {
            if (evlist[i].events & EPOLLIN) {
                if (evlist[i].data.fd == server_fd_tcp) {
                    printf("TCP fd got a message\n");
                    process_tcp(server_fd_tcp, address_tcp);
                }
                if (evlist[i].data.fd == server_fd_udp) {
                    printf("UDP fd got a message\n");
                    process_udp(server_fd_udp);
                }
            }
        }
    }

    close(server_fd_tcp);
    close(server_fd_udp);
}

void process_tcp(int server_fd, struct sockaddr_in address) {
    int new_socket;
    char *response = "Hi!";
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Client connected from %s:%d\n", client_ip, ntohs(address.sin_port));
    
    ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    printf("Received from client: %s\n", buffer);
    
    ssize_t bytes_sent = send(new_socket, response, strlen(response), 0);
    if (bytes_sent < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }
    printf("Sent to client: %s\n", response);
    close(new_socket);
}

void process_udp(int server_fd) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "Hi!";

    ssize_t bytes_received = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
    if (bytes_received == -1) {
        perror("recvfrom");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Client connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));
    printf("Received from client: %s\n", buffer);

    ssize_t bytes_sent = sendto(server_fd, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_addr_len);
    if (bytes_sent == -1) {
        perror("sendto");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}

void init_tcp_fd(int* server_fd_tcp, struct sockaddr_in* address_tcp) {
    int server_fd;
    struct sockaddr_in address;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_TCP);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    *server_fd_tcp = server_fd;
    *address_tcp = address;
}

void init_udp_fd(int* server_fd_udp, struct sockaddr_in* address_udp) {
    int server_fd;
    struct sockaddr_in address;

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_UDP);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    *server_fd_udp = server_fd;
    *address_udp = address;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, sigint_handler);

    if (argc != 2) {
        printf("Error: choose --select, --poll or --epoll flag\n./prog_name --flag\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp("--select", argv[1]) == 0) {
        run_server_select();
    } else if (strcmp("--poll", argv[1]) == 0) {
        run_server_poll();
    } else if (strcmp("--epoll", argv[1]) == 0) {
        run_server_epoll();
    }

    return 0;
}