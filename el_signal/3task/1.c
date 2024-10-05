#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <bits/sigaction.h>

int main(void)
{

    printf("PID процесса: %d\n", getpid());
    sigset_t mask;

    // Инициализировать маску сигналов
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    // Блокировать сигнал SIGUSR1
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    printf("Сигнал SIGUSR1 заблокирован. Переходим в бесконечный цикл ожидания сигнала...\n");

    while (1) {
        int sig;

        // Ожидать сигнала SIGUSR1
        if (sigwait(&mask, &sig) == -1) {
            perror("sigwait");
            exit(EXIT_FAILURE);
        }

        // Если получен сигнал SIGUSR1, вывести сообщение
        if (sig == SIGUSR1) {
            printf("Получен сигнал SIGUSR1!\n");
        }
    }

    return 0;
}