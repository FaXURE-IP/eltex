#include <stdio.h>
#include <pthread.h>


void *print_index(void *arg)
{
    int *i = (int *)arg;
    printf("%d\n", *i);
    return NULL;
}

void create_threads()
{
    pthread_t threads[5];
    int indexes[5];

    for (int i = 0; i < 5; i++)
    {
        indexes[i] = i;

        pthread_create(&threads[i], NULL, print_index, (void*)&indexes[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }
}
int main(){
    create_threads();
    return 0;
}