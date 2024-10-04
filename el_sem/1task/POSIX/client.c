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
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);

    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    char *shared_memory = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shared_memory == MAP_FAILED)
    {
        perror("mmap");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server says: %s\n", shared_memory);

    strcpy(shared_memory, "Hello!");

    munmap(shared_memory, SHARED_MEMORY_SIZE);
    close(shm_fd);
    return 0;
}