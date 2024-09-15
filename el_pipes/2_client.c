#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define PIPE_NAME "/tmp/mypipe"
#define BUFFER_SIZE 32

int main()
{
    int pipe_fd = open(PIPE_NAME, O_RDONLY);
    if (pipe_fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    if (read(pipe_fd, buffer, sizeof(buffer)) == -1)
    {
        perror("read");
        close(pipe_fd);
        exit(EXIT_FAILURE);
    }

    printf("%s\n", buffer);

    close(pipe_fd);

    if (unlink(PIPE_NAME) == -1)
    {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}