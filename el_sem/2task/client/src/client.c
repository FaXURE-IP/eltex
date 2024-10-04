#include "../include/messanger.h"

ChatRoom *chatroom;
int sem_id;
char current_user[MAX_NAME_LEN];
char input_message[MAX_message_LEN];
int key_catcher[3];

WINDOW *message_win, *user_win, *input_win;
WINDOW *message_win_border, *user_win_border, *input_win_border;
int message_win_height, message_win_width;
int user_win_height, user_win_width;
int input_win_height, input_win_width;

int message_scroll_pos;
int user_scroll_pos;
int active_window; // 0 - message_win, 1 - user_win

pthread_t receive_thread;

void init_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    keypad(input_win, TRUE);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
}
void create_windows() {
    message_win_height = LINES - 5;
    message_win_width = COLS - 24;
    user_win_height = LINES - 5;
    user_win_width = 20;
    input_win_height = 4;
    input_win_width = COLS;

    message_win_border = newwin(message_win_height + 1, message_win_width + 2, 0, 0);
    user_win_border = newwin(user_win_height + 1, user_win_width + 2, 0, COLS - user_win_width - 2);
    input_win_border = newwin(input_win_height, input_win_width, LINES - input_win_height, 0);

    message_win = newwin(message_win_height - 1, message_win_width, 1, 1);
    user_win = newwin(user_win_height - 1, user_win_width, 1, COLS - user_win_width - 1);
    input_win = newwin(input_win_height - 2, input_win_width - 2, LINES - input_win_height + 1, 1);

    scrollok(message_win, TRUE);
    scrollok(user_win, TRUE);
    scrollok(input_win, TRUE);

    box(message_win_border, 0, 0);
    box(user_win_border, 0, 0);
    box(input_win_border, 0, 0);

    wrefresh(message_win_border);
    wrefresh(user_win_border);
    wrefresh(input_win_border);
}
void display_messages() {
    werase(message_win);
    int start = chatroom->message_count - message_scroll_pos - message_win_height / 2;

    if (start < 0) start = 0;
    for (int i = start; i < start + message_win_height / 2; i++) {
        if (i >= chatroom->message_count) break;

        wattron(message_win, COLOR_PAIR(1));
        wprintw(message_win, "%s: ", chatroom->messages[i].username);
        wattroff(message_win, COLOR_PAIR(1));
        wattron(message_win, COLOR_PAIR(2));
        wprintw(message_win, "%s\n", chatroom->messages[i].message);
        wattroff(message_win, COLOR_PAIR(2));
        wattron(message_win, COLOR_PAIR(3));
        wprintw(message_win, "%s\n", chatroom->messages[i].datetime);
        wattroff(message_win, COLOR_PAIR(3));
    }
    wrefresh(message_win);
}

void display_users() {
    werase(user_win);
    int start_index = user_scroll_pos;
    int max_displayed_users = user_win_height;
    for (int i = start_index; i < start_index + max_displayed_users; i++) {
        if (i >= chatroom->num_clients) break;

        wprintw(user_win, "%s\n", chatroom->clients[i]);
    }
    wrefresh(user_win);
}

void display_input() {
    werase(input_win);
    wattron(input_win, COLOR_PAIR(4));
    mvwprintw(input_win, 1, 1, "%s", input_message);
    wattroff(input_win, COLOR_PAIR(4));
    wrefresh(input_win);
}

void catch(const char ch) {
    key_catcher[0] = key_catcher [1];
    key_catcher[1] = key_catcher[2];
    key_catcher[2] = ch;
}

bool is_key_up(){
    return key_catcher[0] == 27 && key_catcher[1] == 91 && key_catcher[2] == 65;
}

bool is_key_down(){
    return key_catcher[0] == 27 && key_catcher[1] == 91 && key_catcher[2] == 66;
}

void handle_input() {
    int ch;
    int input_index = strlen(input_message);

    while ((ch = wgetch(input_win)) != '\n') {
        catch(ch);

        if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (input_index > 0) {
                input_message[--input_index] = '\0';
            }
        } else if (ch == KEY_RESIZE) {
            create_windows();
            display_messages();
            display_users();
            display_input();
        } else if (ch == '\t') {
            switch_window();
        } else if (is_key_up()) {
            input_index -= 2;
            input_message[input_index] = '\0';

            if (active_window == 0 && message_scroll_pos < chatroom->message_count - message_win_height / 2) {
                message_scroll_pos++;
                display_messages();
            } else if (active_window == 1 && user_scroll_pos < chatroom->num_clients - user_win_height) {
                user_scroll_pos++;
                display_users();
            }
        } else if (is_key_down()) {
            input_index -= 2;
            input_message[input_index] = '\0';

            if (active_window == 0 && message_scroll_pos > 0) {
                message_scroll_pos--;
                display_messages();
            } else if (active_window == 1 && user_scroll_pos > 0) {
                user_scroll_pos--;
                display_users();
            }
        } else if (input_index < MAX_message_LEN - 1) {
            input_message[input_index++] = ch;
            input_message[input_index] = '\0';
        }
        display_input();
    }

    if (input_index > 0) {
        add_message(current_user, input_message);
        input_message[0] = '\0';
        display_messages();
        display_input();
    }
}

void clean_up() {
    endwin();
    increment_semaphore(sem_id);
    for (int i = 0; i < chatroom->num_clients; i++) {
        if (strcmp(chatroom->clients[i], current_user) == 0) {
            for (int j = i; j < chatroom->num_clients - 1; j++) {
                strcpy(chatroom->clients[j], chatroom->clients[j + 1]);
            }
            chatroom->num_clients--;
            break;
        }
    }
    decrement_semaphore(sem_id);
    shmdt(chatroom);
    exit(0);
}

void *receive_messages(void *arg) {
    int prev_message_count = chatroom->message_count;
    int prev_user_count = chatroom->num_clients;
    while (1) {
        increment_semaphore(sem_id);
        if (chatroom->message_count != prev_message_count) {
            prev_message_count = chatroom->message_count;
            display_messages();
        }
        if (chatroom->num_clients != prev_user_count) {
            prev_user_count = chatroom->num_clients;
            display_users();
        }
        decrement_semaphore(sem_id);
        sleep(1);
    }
    return NULL;
}

void decrement_semaphore(int semaphore_id) {
    struct sembuf operation = {0, -1, SEM_UNDO};
    semop(semaphore_id, &operation, 1);
}

void increment_semaphore(int sem_id) {
    struct sembuf increment = {0, 1, SEM_UNDO};
    semop(sem_id, &increment, 1);
}

void add_message(const char *username, const char *message) {
    increment_semaphore(sem_id);
    strncpy(chatroom->messages[chatroom->message_count].username, username, MAX_NAME_LEN);
    strncpy(chatroom->messages[chatroom->message_count].message, message, MAX_message_LEN);
    time_t now = time(NULL);
    strftime(chatroom->messages[chatroom->message_count].datetime, MAX_DATE_LEN, "%Y-%m-%d %H:%M:%S", localtime(&now));
    chatroom->message_count++;
    decrement_semaphore(sem_id);
}
void switch_window() {
    active_window = (active_window + 1) % 2;
}

void cleanup(int signum) {
    clean_up();
}

void run_client(const char *username) {
    strcpy(current_user, username);
    signal(SIGINT, cleanup);
    key_catcher[0] = 0;
    key_catcher[1] = 0;
    key_catcher[2] = 0;

    int shm_id = shmget(SHM_KEY, sizeof(ChatRoom), 0666);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    chatroom = (ChatRoom *)shmat(shm_id, NULL, 0);
    if (chatroom == (ChatRoom *)-1) {
        perror("shmat");
        exit(1);
    }

    sem_id = semget(SEM_KEY, 1, 0666);
    if (sem_id < 0) {
        perror("semget");
        exit(1);
    }

    init_ncurses();

    decrement_semaphore(sem_id);
    strncpy(chatroom->clients[chatroom->num_clients], current_user, MAX_NAME_LEN);
    chatroom->num_clients++;
    increment_semaphore(sem_id);

    create_windows();
    display_messages();
    display_users();
    display_input();

    pthread_create(&receive_thread, NULL, receive_messages, NULL);

    while (1) {
        handle_input();
    }
}