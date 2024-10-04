#ifndef MESSANGER_H
#define MESSANGER_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

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

extern ChatRoom *chatroom;
extern int sem_id;
extern char current_user[MAX_NAME_LEN];
extern char input_message[MAX_message_LEN];

extern WINDOW *message_win, *user_win, *input_win;
extern WINDOW *message_win_border, *user_win_border, *input_win_border;
extern int message_win_height, message_win_width;
extern int user_win_height, user_win_width;
extern int input_win_height, input_win_width;

extern int message_scroll_pos;
extern int user_scroll_pos;
extern int active_window; // 0 - message_win, 1 - user_win

extern pthread_t receive_thread;

void run_client(const char *username);
void init_ncurses();
void create_windows();
void display_messages();
void display_users();
void display_input();
void handle_input();
void clean_up();
void *receive_messages(void *arg);
void decrement_semaphore(int sem_id);
void increment_semaphore(int sem_id);
void add_message(const char *username, const char *message);
void switch_window();
void cleanup(int signum);

#endif // MESSANGER_H