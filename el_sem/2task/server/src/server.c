#include "../include/server.h"


int shm_id;
int sem_id;

ChatRoom *chatroom;

void clean(int signal_number) {
    shmdt(chatroom);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}


void decrement_semaphore(int semaphore_id) {
    struct sembuf operation = {0, -1, SEM_UNDO};
    semop(semaphore_id, &operation, 1);
}

void increment_semaphore(int semaphore_id) {
    struct sembuf operation = {0, 1, SEM_UNDO};
    semop(semaphore_id, &operation, 1);
}

void run_server(void) {
    signal(SIGINT, clean);

    shm_id = shmget(SHM_KEY, sizeof(ChatRoom), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    chatroom = shmat(shm_id, NULL, 0);
    if (chatroom == (ChatRoom *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    semctl(sem_id, 0, SETVAL, 0);

    chatroom->num_clients = 0;
    chatroom->message_count = 0;

    while (1) {
        sleep(0.1);
    }
}
