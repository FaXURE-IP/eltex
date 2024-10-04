#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>

#define MAX_NAME_LEN 32
#define MAX_message_LEN 512
#define MAX_CLIENTS 100
#define MAX_MESSAGES 1000
#define MAX_DATE_LEN 64
#define SHM_KEY 0x1234
#define SEM_KEY 0x5678

typedef struct {
    char username[MAX_NAME_LEN];
    char message[MAX_message_LEN];
    char datetime[MAX_DATE_LEN];
} Message;

typedef struct {
    int num_clients;
    char clients[MAX_CLIENTS][MAX_NAME_LEN];
    Message messages[MAX_MESSAGES];
    int message_count;
} ChatRoom;

extern int shm_id;
extern int semaphore_id;
extern ChatRoom *chatroom;

void run_server();
void clean(int signum);
void decrement_semaphore(int semaphore_id);
void increment_semaphore(int semaphore_id);

#endif // SERVER_H