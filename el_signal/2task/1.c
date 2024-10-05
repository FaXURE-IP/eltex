#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <asm-generic/signal.h>

int main(void)
{
    printf("PID процесса: %d\n", getpid());
    sigset_t set;

    int status, sig_num;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    status = sigprocmask(SIG_BLOCK, &set, NULL);

    if (status == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }  

    while(1){
        sigwait(&set, &sig_num);

        printf("\nGot signal %d\n", sig_num);
    }

    return 0;
}