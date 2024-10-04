#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <asm-generic/signal.h>

void sig_handler(int sig_num);

int main(void)
{
    struct sigaction sa;
    int status;

    sa.sa_handler = sig_handler;
    status = sigaction(SIGUSR1, &sa, NULL);

    if (status < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sleep(1);
    }
}

void sig_handler(int sig_num) {
  printf("Signal SIGUSR1 - %d\n", sig_num);
}