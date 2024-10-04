#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SHARED_MEMORY_NAME "/my_shared_memory"
#define SHARED_MEMORY_SIZE 256

int main()
{
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, SHARED_MEMORY_SIZE) == -1) {
        perror("ftruncate");
        close(shm_fd);
        shm_unlink(SHARED_MEMORY_NAME);
        exit(1);
    }

    char *shared_memory = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shared_memory == MAP_FAILED)
    {
        perror("mmap");
        close(shm_fd);
        shm_unlink(SHARED_MEMORY_NAME);
        exit(EXIT_FAILURE);
    }

    strcpy(shared_memory, "Hi!");

    while (strcmp(shared_memory, "Hello!") != 0)
    {
        sleep(1);
    }

    printf("Client says: %s\n", shared_memory);

    munmap(shared_memory, SHARED_MEMORY_SIZE);
    close(shm_fd);
    unlink(SHARED_MEMORY_NAME);

    return 0;
}