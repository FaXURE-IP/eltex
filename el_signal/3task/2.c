#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sched.h>

int main(void) {
    pid_t pid;

    printf("Enter the PID of the process to send signal to: ");
    scanf("%d", &pid);

    if (kill(pid, SIGUSR1) == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
    }

    printf("Signal SIGUSR1 sent to process %d\n", pid);

    return 0;
}