#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 12345
#define SHM_SIZE 256

int main()
{
    int shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1)
    {
        perror("shmget");
        exit(1);
    }

    char *shared_memory = (char *)shmat(shm_id, NULL, 0);
    if (shared_memory == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    strcpy(shared_memory, "Hi!");

    while (strcmp(shared_memory, "Hello!") != 0)
    {
        sleep(1);
    }

    printf("Client says: %s\n", shared_memory);

    if (shmdt(shared_memory) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        exit(1);
    }

    return 0;
}
