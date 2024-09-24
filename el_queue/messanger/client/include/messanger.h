#ifndef MESSANGER_H
#define MESSANGER_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <sys/ipc.h>
#include <errno.h>
#include <pthread.h>
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

extern User user_info;

extern int regitration_queue;

typedef struct
{
    char username[MAX_NAME_LEN];
    char message[MAX_MSG_LEN];
    char datetime[MAX_DATE_LEN];
} Message;

extern Message *messages;
extern int messages_size;

extern User *users;
extern int users_size;

void run_messanger(const char *username);
void init_ncurses();
void create_windows();
void add_message(Message message, bool is_new);
void add_user(User user);
void display_messages();
void display_users();
void display_input();
void *handle_input(void *);
void read_message();
void switch_window();
void clean_exit();
void *handle(void *);

#endif // MESSANGER_H