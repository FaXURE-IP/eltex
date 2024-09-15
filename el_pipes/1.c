#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUFF_SIZE 32

int main()
{
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe error");
        exit(-1);
    }

    int child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork error");
        exit(-1);
    }

    if (child_pid == 0)
    {
        char messege_to_send[] = "Hi!\n";
        write(pipe_fd[1], messege_to_send, strlen(messege_to_send));
        printf("Data send to child\n");
        exit(0);
    }
    else
    {
        wait(NULL);
        char messege_to_recieve[BUFF_SIZE];
        read(pipe_fd[0], messege_to_recieve, BUFF_SIZE);
        printf("Parent: Data from child = %s\n", messege_to_recieve);
        exit(0);
    }

    return 0;
}