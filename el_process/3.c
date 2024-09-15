#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_CMDS 16

void execute_command(char *cmd)
{
    char *args[MAX_ARGS];
    char *token = strtok(cmd, " ");
    int i = 0;

    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (execvp(args[0], args) == -1)
    {
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    char input[MAX_INPUT];
    char *cmds[MAX_CMDS];
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

        int num_cmds = 0;
        char *token = strtok(input, "|");
        while (token != NULL && num_cmds < MAX_CMDS - 1)
        {
            cmds[num_cmds] = token;
            num_cmds++;
            token = strtok(NULL, "|");
        }
        cmds[num_cmds] = NULL;

        int pipefds[2 * (num_cmds - 1)];

        for (int i = 0; i < num_cmds - 1; i++)
        {
            if (pipe(pipefds + i * 2) < 0)
            {
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < num_cmds; i++)
        {
            pid = fork();
            if (pid < 0)
            {
                perror("fork failed");
                exit(EXIT_FAILURE);
            }

            if (pid == 0)
            {

                if (i > 0)
                {
                    if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0)
                    {
                        perror("dup2 failed (input)");
                        exit(EXIT_FAILURE);
                    }
                }


                if (i < num_cmds - 1)
                {
                    if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0)
                    {
                        perror("dup2 failed (output)");
                        exit(EXIT_FAILURE);
                    }
                }

                for (int j = 0; j < 2 * (num_cmds - 1); j++)
                {
                    close(pipefds[j]);
                }

                execute_command(cmds[i]);
            }
        }

        for (int i = 0; i < 2 * (num_cmds - 1); i++)
        {
            close(pipefds[i]);
        }
        for (int i = 0; i < num_cmds; i++)
        {
            wait(&status);
        }
    }

    return 0;
}
