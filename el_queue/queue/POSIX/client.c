#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define SERVER_QUEUE "/server"
#define CLIENT_QUEUE "/client"
#define MAX_SIZE 1024

int main()
{
    mqd_t mes_q1, mes_q2;
    char bufferfer[MAX_SIZE];

    mes_q1 = mq_open(SERVER_QUEUE, O_RDONLY);
    if (mes_q1 == -1)
    {
        perror("mes_q1 open");
        exit(1);
    }

    mes_q2 = mq_open(CLIENT_QUEUE, O_WRONLY);

    if (mes_q2 == -1)
    {
        perror("mes_q1 open");
        exit(1);
    }

    if(mq_receive(mes_q1, bufferfer, MAX_SIZE, NULL) == -1){
        perror("mq_recieve");
        exit(1);
    }

    printf("Recieved from server %s\n",bufferfer);

    strcpy(bufferfer, "Hello!");

    if(mq_send(mes_q2,bufferfer,strlen(bufferfer) + 1, 0) == -1){
        perror("mq_send");
    }

    if (mq_close(mes_q1) == -1)
    {
        perror("mq_close");
        exit(1);
    }

    if (mq_close(mes_q2) == -1)
    {
        perror("mq_close");
        exit(1);
    }

}