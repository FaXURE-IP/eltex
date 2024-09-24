#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>

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

    if (msgrcv(msgid, &message, sizeof(message.mtext), 1, 0) == -1)
    {
        printf("msgrcv error: %s\n", strerror(errno));
        exit(1);
    }

    printf("Received from client: %s\n", message.mtext);

    message.mtype = 2;
    strcpy(message.mtext, "Hello!");
    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
    {
        perror("msgsnd");
        exit(1);
    }

    return 0;
}
