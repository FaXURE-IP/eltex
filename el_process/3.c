#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main()
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    char *token;
    int status;
    pid_t pid;

    while (1)
    {
        printf("my_shell> ");
        fgets(input, MAX_INPUT, stdin);

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0)
        {
            break;
        }
        int i = 0;
        token = strtok(input, " ");
        while (token != NULL && i < MAX_ARGS - 1)
        {
            args[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            if (execvp(args[0], args) == -1)
            {
                perror("execvp failed");
            }
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
