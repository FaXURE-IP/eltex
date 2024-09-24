#include "../include/server.h"

User *users;
int users_size;

Message *messages;
int messages_size;

int regitration_queue;
int id;

void run_server()
{

    create_file(REG_QUEUE_NAME);
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

    printf("Server started and waiting for client registrations on descript = %d...\n", regitration_queue);

    handle();

    clean_exit();
}

void add_user(User user)
{
    User *temp = (User *)realloc(users, (++users_size) * sizeof(User));
    if (temp != NULL || (temp == NULL && users_size == 0))
    {
        users = temp;
    }
    else
    {
        perror("realloc");
        free(users);
        users = NULL;
    }

    strcpy(users[users_size - 1].username, user.username);
    users[users_size - 1].descript = user.descript;
    users[users_size - 1].id = user.id;
}

void add_message(Message message)
{
    Message *temp = (Message *)realloc(messages, (++messages_size) * sizeof(Message));
    if (temp != NULL || (temp == NULL && messages_size == 0))
    {
        messages = temp;
    }
    else
    {
        perror("realloc");
        free(messages);
        users = NULL;
    }

    strcpy(messages[messages_size - 1].username, message.username);
    strcpy(messages[messages_size - 1].message, message.message);
    strcpy(messages[messages_size - 1].datetime, message.datetime);
}

void handle()
{
    Msgbuffer msg;
    User user;
    Message message;

    while (1)
    {
        if (msgrcv(regitration_queue, &msg, sizeof(Msgbuffer) - sizeof(long), CLIENT, IPC_NOWAIT) >= 0)
        {
            switch (msg.msg_type)
            {
            case REG:
                printf("User %s asks for registration.\n", msg.username);
                strcpy(user.username, msg.username);
                registeration(user);
                break;
            }
        }
        for (int i = 0; i < users_size; i++)
        {
            if (msgrcv(users[i].descript, &msg, sizeof(Msgbuffer) - sizeof(long), CLIENT, IPC_NOWAIT) >= 0)
            {
                switch (msg.msg_type)
                {
                case MSGINFO:
                    printf("Received message from %s: %s\n", msg.username, msg.message);
                    msg.mtype = SERVER;
                    broadcast(msg, users[i].id);

                    strcpy(message.username, msg.username);
                    strcpy(message.message, msg.message);
                    strcpy(message.datetime, msg.datetime);
                    add_message(message);
                    break;
                }
            }
        }
        sleep(0.1);
    }
}

void broadcast(Msgbuffer buffer, int exeption_id)
{
    for (int i = 0; i < users_size; i++)
    {
        if (users[i].id == exeption_id)
        {
            continue;
        }

        if (msgsnd(users[i].descript, &buffer, sizeof(Msgbuffer) - sizeof(long), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
    }
}

int registeration(User user)
{
    Msgbuffer response;

    user.id = id++;
    char filename[20] = "";
    sprintf(filename, "user_%d", user.id);
    create_file(filename);

    key_t key = ftok(filename, 10);
    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }

    user.descript = msgget(key, 0666 | IPC_CREAT);
    if (user.descript == -1)
    {
        perror("msgget");
        exit(1);
    }

    add_user(user);

    response.descript = user.descript;
    response.mtype = SERVER;
    response.msg_type = USERINFO;
    response.id = user.id;
    strcpy(response.username, user.username);

    if (msgsnd(regitration_queue, &response, sizeof(Msgbuffer) - sizeof(long), 0) == -1)
    {
        perror("msgsnd");
        exit(1);
    }

    send_to_user(users[users_size - 1]);
    broadcast(response, user.id);

    return user.descript;
}

void send_to_user(User user)
{
    Msgbuffer response;

    for (int i = 0; i < messages_size; i++)
    {
        strcpy(response.message, messages[i].message);
        strcpy(response.username, messages[i].username);
        strcpy(response.datetime, messages[i].datetime);
        response.mtype = SERVER;
        response.msg_type = MSGINFO;

        if (msgsnd(user.descript, &response, sizeof(Msgbuffer) - sizeof(long), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
    }

    for (int i = 0; i < users_size; i++)
    {
        strcpy(response.username, users[i].username);
        response.id = users[i].id;
        response.descript = users[i].descript;
        response.mtype = SERVER;
        response.msg_type = USERINFO;

        if (msgsnd(user.descript, &response, sizeof(Msgbuffer) - sizeof(long), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
    }
}

void clean_exit()
{
    if (msgctl(regitration_queue, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }

    printf("Server stopped. Cleaning up and exiting...\n");
    exit(0);
}

void create_file(const char *filename)
{
    if (access(filename, F_OK) == -1)
    {
        int fd = open(filename, O_CREAT | O_EXCL, 0666);
        if (fd == -1)
        {
            perror("open");
            exit(1);
        }
        close(fd);
    }
}