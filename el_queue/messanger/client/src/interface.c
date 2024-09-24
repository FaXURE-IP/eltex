#include "../include/messanger.h"

Message *messages;
int messages_size;

User *users;
int users_size;

char input_message[MAX_MSG_LEN];

WINDOW *message_window, *user_window, *input_window;
WINDOW *message_window_border, *user_window_border, *input_window_border;
int message_window_height, message_window_width;
int user_window_height, user_window_width;
int input_window_height, input_window_width;
int key_catcher[3];

int msg_scroll_posotion;
int user_scroll_posotion;
int active_window; // 0 - message_window, 1 - user_window

void init_ncurses()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Username
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Text Message
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Time
    init_pair(4, COLOR_WHITE, COLOR_BLACK);  // Input window
    init_pair(5, COLOR_WHITE, COLOR_BLACK);  // Border
}

void create_windows()
{
    message_window_height = LINES - 5;
    message_window_width = COLS - 24;
    user_window_height = LINES - 5;
    user_window_width = 20;
    input_window_height = 4;
    input_window_width = COLS;

    message_window_border = newwin(message_window_height + 1, message_window_width + 2, 0, 0);
    user_window_border = newwin(user_window_height + 1, user_window_width + 2, 0, COLS - user_window_width - 2);
    input_window_border = newwin(input_window_height, input_window_width, LINES - input_window_height, 0);

    message_window = newwin(message_window_height - 1, message_window_width, 1, 1);
    user_window = newwin(user_window_height - 1, user_window_width, 1, COLS - user_window_width - 1);
    input_window = newwin(input_window_height - 2, input_window_width - 2, LINES - input_window_height + 1, 1);

    scrollok(message_window, TRUE);
    scrollok(user_window, TRUE);
    scrollok(input_window, TRUE);

    box(message_window_border, 0, 0);
    box(user_window_border, 0, 0);
    box(input_window_border, 0, 0);

    wrefresh(message_window_border);
    wrefresh(user_window_border);
    wrefresh(input_window_border);
}

void display_users()
{
    werase(user_window);
    int start = user_scroll_posotion;
    for (int i = start; i < users_size; i++)
    {
        if (i >= users_size)
            break;

        wprintw(user_window, "%s\n", users[i].username);
    }
    wrefresh(user_window);
}

void display_input()
{
    werase(input_window);
    wattron(input_window, COLOR_PAIR(4));
    mvwprintw(input_window, 1, 1, "%s", input_message);
    wattroff(input_window, COLOR_PAIR(4));
    wrefresh(input_window);
}

void *handle_input(void *)
{
    while (1)
    {
        read_message();
    }
}

void catch (const char ch)
{
    key_catcher[0] = key_catcher[1];
    key_catcher[1] = key_catcher[2];
    key_catcher[2] = ch;
}

bool is_key_up()
{
    return key_catcher[0] == 27 && key_catcher[1] == 91 && key_catcher[2] == 65;
}

bool is_key_down()
{
    return key_catcher[0] == 27 && key_catcher[1] == 91 && key_catcher[2] == 66;
}

void display_messages()
{
    werase(message_window);
    int start = messages_size - msg_scroll_posotion - message_window_height / 2;
    if (start < 0)
        start = 0;
    for (int i = start; i < start + message_window_height / 2; i++)
    {
        if (i >= messages_size)
            break;

        wattron(message_window, COLOR_PAIR(1));
        wprintw(message_window, "%s: ", messages[i].username);
        wattroff(message_window, COLOR_PAIR(1));
        wattron(message_window, COLOR_PAIR(2));
        wprintw(message_window, "%s\n", messages[i].message);
        wattroff(message_window, COLOR_PAIR(2));
        wattron(message_window, COLOR_PAIR(3));
        wprintw(message_window, "%s\n", messages[i].datetime);
        wattroff(message_window, COLOR_PAIR(3));
    }
    wrefresh(message_window);
}

void read_message()
{
    int ch;
    int input_index = strlen(input_message);

    while ((ch = wgetch(input_window)) != '\n')
    {
        catch (ch);

        if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b')
        {
            if (input_index > 0)
            {
                input_message[--input_index] = '\0';
            }
        }
        else if (ch == KEY_RESIZE)
        {
            create_windows();
            display_messages();
            display_users();
            display_input();
        }
        else if (ch == '\t')
        {
            switch_window();
        }
        else if (is_key_up())
        {
            input_index -= 2;
            input_message[input_index] = '\0';

            if (active_window == 0 && msg_scroll_posotion < messages_size - message_window_height / 2)
            {
                msg_scroll_posotion++;
                display_messages();
            }
            else if (active_window == 1 && user_scroll_posotion < users_size - user_window_height)
            {
                user_scroll_posotion++;
                display_users();
            }
        }
        else if (is_key_down())
        {
            input_index -= 2;
            input_message[input_index] = '\0';

            if (active_window == 0 && msg_scroll_posotion > 0)
            {
                msg_scroll_posotion--;
                display_messages();
            }
            else if (active_window == 1 && user_scroll_posotion > 0)
            {
                user_scroll_posotion--;
                display_users();
            }
        }
        else if (input_index < MAX_MSG_LEN - 1)
        {
            input_message[input_index++] = ch;
            input_message[input_index] = '\0';
        }
        display_input();
    }

    if (input_index > 0)
    {
        Message msg;
        strcpy(msg.username, user_info.username);
        strcpy(msg.message, input_message);
        add_message(msg, 1);
        input_message[0] = '\0';
        display_messages();
        display_input();
    }
}

void switch_window()
{
    active_window = (active_window + 1) % 2;
}

void run_messanger(const char *username)
{
    strcpy(input_message, "");
    key_catcher[0] = 0;
    key_catcher[1] = 0;
    key_catcher[2] = 0;

    Msgbuffer reg_message, response;
    key_t reg_key = ftok(REG_QUEUE_NAME, 10);

    if (reg_key == -1)
    {
        perror("ftok");
        exit(1);
    }

    regitration_queue = msgget(reg_key, 0666 | IPC_CREAT);
    if (regitration_queue == -1)
    {
        perror("msgget");
        exit(1);
    }

    printf("Client started on descript = %d...\n", regitration_queue);

    reg_message.mtype = CLIENT;
    reg_message.msg_type = REG;
    strcpy(reg_message.username, username);
    reg_message.id = getpid();

    if (msgsnd(regitration_queue, &reg_message, sizeof(Msgbuffer) - sizeof(long), 0) == -1)
    {
        perror("msgsnd");
        exit(1);
    }

    if (msgrcv(regitration_queue, &response, sizeof(Msgbuffer) - sizeof(long), SERVER, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }

    user_info.descript = response.descript;
    user_info.id = response.id;
    strcpy(user_info.username, response.username);

    init_ncurses();
    create_windows();
    display_messages();
    display_users();
    display_input();

    pthread_t input_thread, server_thread;

    if (pthread_create(&input_thread, NULL, handle, NULL) != 0)
    {
        perror("Failed to create input thread");
        endwin();
        return EXIT_FAILURE;
    }
    if (pthread_create(&server_thread, NULL, handle_input, NULL) != 0)
    {
        perror("Failed to create server thread");
        endwin();
        return EXIT_FAILURE;
    }

    pthread_join(input_thread, NULL);
    pthread_cancel(server_thread);

    clean_exit();
}

void clean_exit()
{
    Msgbuffer exit_message;

    strcpy(exit_message.username, user_info.username);
    exit_message.mtype = CLIENT;
    exit_message.id = user_info.id;
    exit_message.descript = user_info.descript;

    if (msgsnd(regitration_queue, &exit_message, sizeof(Msgbuffer) - sizeof(long), 0) == -1)
    {
        perror("msgsnd");
        exit(1);
    }

    free(messages);
    free(users);
    endwin();
    exit(0);
}

void *handle(void *)
{
    Msgbuffer msg;
    User user;
    Message message;

    while (1)
    {
        if (msgrcv(user_info.descript, &msg, sizeof(Msgbuffer) - sizeof(long), SERVER, IPC_NOWAIT) >= 0)
        {
            switch (msg.msg_type)
            {
            case MSGINFO:
                strcpy(message.username, msg.username);
                strcpy(message.message, msg.message);
                strcpy(message.datetime, msg.datetime);

                add_message(message, 0);
                display_messages();
                break;
            case USERINFO:
                strcpy(user.username, msg.username);
                user.id = msg.id;
                user.descript = msg.descript;

                add_user(user);
                break;
            }
        }
        sleep(0.1);
    }
}