#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void create_process_and_wait(int process_number)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("Process%d: PID = %d, PPID = %d\n", process_number, getpid(), getppid());

        if (process_number == 1)
        {

            create_process_and_wait(3);
            create_process_and_wait(4);
        }
        else if (process_number == 2)
        {

            create_process_and_wait(5);
        }

        exit(EXIT_SUCCESS);
    }
    else
    {
        wait(NULL);
    }
}

int main()
{
    printf("Main Process: PID = %d, PPID = %d\n", getpid(), getppid());

    create_process_and_wait(1);
    create_process_and_wait(2);

    return 0;
}
