#include "../include/messanger.h"

User user_info;

int regitration_queue;

void add_message(Message message, bool is_new)
{
    Message *temp = (Message *)realloc(messages, (++messages_size) * sizeof(Message));
    if (temp != NULL || (temp == NULL && messages_size == 0))
    {
        messages = temp;
    }
    else
    {
        free(messages);
        users = NULL;
    }

    strcpy(messages[messages_size - 1].username, message.username);
    strcpy(messages[messages_size - 1].message, message.message);

    if (is_new)
    {
        time_t now = time(NULL);
        strftime(messages[messages_size - 1].datetime, MAX_DATE_LEN, "%Y-%m-%d %H:%M:%S", localtime(&now));

        Msgbuffer buffer;
        buffer.mtype = CLIENT;
        buffer.msg_type = MSGINFO;
        strcpy(buffer.username, message.username);
        strcpy(buffer.datetime, messages[messages_size - 1].datetime);
        strcpy(buffer.message, message.message);
        if (msgsnd(user_info.descript, &buffer, sizeof(Msgbuffer) - sizeof(long), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
    }
    else
    {
        strcpy(messages[messages_size - 1].datetime, message.datetime);
    }
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

    display_users();
}