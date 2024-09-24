#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define QUEUE_PERMISSIONS 0666
#define N 100

struct msgbuffer
{
    long mtype;
    char mtext[N];
};

int main()
{
    key_t key;
    int msgid;

    struct msgbuffer message;

    int fd = open("my_queue", O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    close(fd);

    key = ftok("my_queue", 50);
    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }

    msgid = msgget(key, IPC_CREAT | QUEUE_PERMISSIONS);
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }

    message.mtype = 1;
    strcpy(message.mtext, "Hi!");
    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
    {
        perror("msgsnd");
        exit(1);
    }

    if (msgrcv(msgid, &message, sizeof(message.mtext), 2, 0) == -1)
    {
        perror("msgrcv");
        exit(1);
    }

    printf("Received from client: %s\n", message.mtext);

    if (msgctl(msgid, IPC_RMID, 0) == -1)
    {
        perror("msgctl");
        exit(1);
    }

    return 0;
}
