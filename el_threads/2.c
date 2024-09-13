#include <stdio.h>
#include <pthread.h>

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
long a = 0;

void *increment(void *arg)
{
    for (int i = 0; i < 25000000; i++)
    {
        a++;
    }
    return NULL;
}

void *increment_mutex(void *arg)
{
    for (int i = 0; i < 50000000; i++)
    {
        pthread_mutex_lock(&m1);
        if (a == 50000000)
        {
            pthread_cond_signal(&cond1);
        }
        else
        {
            a++;
        }
        pthread_mutex_unlock(&m1);
    }
    return NULL;
}

void *increment_watch(void *arg)
{
    pthread_mutex_lock(&m1);
    pthread_cond_wait(&cond1, &m1);
    pthread_mutex_unlock(&m1);
    return NULL;
}

int main()
{
    pthread_t thread[3];

    // Create threads without mutex
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&thread[i], NULL, increment, NULL);
    }

    pthread_create(&thread[2], NULL, increment_watch, NULL);

    // Wait for threads to finish
    for (int j = 0; j < 2; j++)
    {
        pthread_join(thread[j], NULL);
    }

    printf("NO MUTEX  a = %ld\n", a);

    a = 0;

    for (int i = 0; i < 2; i++)
    {
        pthread_create(&thread[i], NULL, increment_mutex, NULL);
    }

    pthread_create(&thread[2], NULL, increment_watch, NULL);

    for (int j = 0; j < 2; j++)
    {
        pthread_join(thread[j], NULL);
    }

    printf("MUTEX  a = %ld\n", a);

    return 0;
}
