#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <malloc.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/select.h>

#define REG_QUEUE_NAME "regitration_queue"
#define MAX_NAME_LEN 32
#define MAX_MSG_LEN 128
#define MAX_DATE_LEN 32

typedef enum
{
    CLIENT = 1,
    SERVER = 2
} SENDER;
typedef enum
{
    REG = 1,
    MSGINFO = 3,
    USERINFO = 4
} MSG_TYPE;

typedef struct
{
    char username[MAX_NAME_LEN];
    char message[MAX_MSG_LEN];
    char datetime[MAX_DATE_LEN];
} Message;

typedef struct
{
    char username[MAX_NAME_LEN];
    mqd_t descript;
    int id;
} User;

typedef struct
{
    long mtype;

    int msg_type;
    mqd_t descript;
    int id;
    char username[MAX_NAME_LEN];
    char message[MAX_MSG_LEN];
    char datetime[MAX_DATE_LEN];
} Msgbuffer;

void run_server();
void handle();
void clean_exit();
void create_file(const char *filename);
int registeration(User user);
void add_user(User user);
void send_to_user(User user);
void add_message(Message msg);
void broadcast(Msgbuffer buffer, int exeption_id);

#endif // SERVER_H