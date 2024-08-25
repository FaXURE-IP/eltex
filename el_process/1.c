#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        printf("Child pid: %d   ppid: %d \n", getpid(), getppid());
    }
    else
    {
        waitpid(pid, 0, 0);
        printf("Paren pid: %d   ppid: %d \n", getpid(), getppid());
    }

    return 0;
}