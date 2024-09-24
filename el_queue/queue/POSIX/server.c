#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define QUEUE_PERMISSIONS 0666
#define SERVER_QUEUE "/server"
#define CLIENT_QUEUE "/client"
#define MAX_SIZE 1024

int main()
{
    mqd_t mes_q1, mes_q2;
    struct mq_attr attr;
    char buffer[MAX_SIZE];

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mes_q1 = mq_open(SERVER_QUEUE, O_CREAT | O_WRONLY, QUEUE_PERMISSIONS, &attr);

    if (mes_q1 == -1)
    {
        perror("mes_q1 open");
        exit(1);
    }

    mes_q2 = mq_open(CLIENT_QUEUE, O_CREAT | O_RDONLY, QUEUE_PERMISSIONS, &attr);

    if (mes_q2 == -1)
    {
        perror("mes_q2 open");
        exit(1);
    }

    strcpy(buffer, "Hi!");

    if (mq_send(mes_q1, buffer, strlen(buffer) + 1, 0) == -1)
    {
        perror(mq_send);
        exit(1);
    }

    if (mq_receive(mes_q2, buffer, MAX_SIZE, 0) == -1)
    {
        perror("mq_receive");
        exit(1);
    }

    printf("recieve from client %s\n", buffer);

    if (mq_close(mes_q1) == -1)
    {
        perror("mq_close");
        exit(1);
    }

    if (mq_unlink(SERVER_QUEUE) == -1)
    {
        perror("mq_unlink server");
        exit(1);
    }

    if (mq_close(mes_q2) == -1)
    {
        perror("mq_close");
        exit(1);
    }
    if (mq_unlink(CLIENT_QUEUE) == -1)
    {
        perror("mq_unlink client");
        exit(1);
    }

    return 0;
}