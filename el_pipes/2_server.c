#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define PIPE_NAME "/tmp/mypipe"
#define MESSAGE "Hi!"
int main()
{
    if (mkfifo(PIPE_NAME, 0666) == -1)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    int pipe_fd = open(PIPE_NAME, O_WRONLY);
    if (pipe_fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (write(pipe_fd, MESSAGE, sizeof(MESSAGE)) == -1)
    {
        perror("write");
        close(pipe_fd);
        exit(EXIT_FAILURE);
    }

    close(pipe_fd);

    return 0;
}