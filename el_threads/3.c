#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define INITIAL_STOCK 10000
#define CUSTOMER_NEEDS 100000
#define LOADER_SUPPLY 5000

int *finish_customers;
int *shop;
int *customers;
int num_of_finished_customers = 0;
int program_finished = 0;

pthread_t *customer_threads;
pthread_t loader_thread;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

int random_offset(int N)
{
  return (rand() % (2 * N + 1)) - N;
}

void trading_watcher(int index, int flag)
{
  pthread_mutex_lock(&m);
  if (flag == 1)
  {
    pthread_cancel(customer_threads[index]);
  }
  else
  {
    pthread_cancel(loader_thread);
  }
  pthread_mutex_unlock(&m);
}

void *customer(void *arg)
{
  int index = *(int *)arg;
  free(arg);

  while (1)
  {
    pthread_mutex_lock(&m);

    if (program_finished)
    {
      pthread_mutex_unlock(&m);
      break;
    }

    if (customers[index] >= CUSTOMER_NEEDS)
    {
      if (finish_customers[index] != -1)
      {
        finish_customers[index] = -1;
        num_of_finished_customers++;
        printf("Customer %d has fulfilled their needs.\n", index);
      }

      if (num_of_finished_customers == 3)
      {
        program_finished = 1;
        printf("All customers have finished.\n");
      }
    }

    if (finish_customers[index] != -1)
    {
      int random_shop = rand() % 5;
      if (shop[random_shop] > 10000)
      {
        customers[index] += 10000;
        shop[random_shop] -= 10000;
        printf("Customer %d entered shop %d. Customer's goods: %d. Remaining stock in shop: %d\n", index, random_shop, customers[index], shop[random_shop]);
        sleep(2);
      }
    }

    pthread_mutex_unlock(&m);
    sleep(1);
  }

  return NULL;
}

void *loader(void *arg)
{
  while (1)
  {
    pthread_mutex_lock(&m);

    if (program_finished)
    {
      pthread_mutex_unlock(&m);
      break;
    }

    int random_shop = rand() % 5;
    shop[random_shop] += LOADER_SUPPLY;
    printf("Loader added 5000 to shop %d. New stock: %d\n", random_shop, shop[random_shop]);
    pthread_mutex_unlock(&m);
    sleep(1);
  }

  return NULL;
}

int main(void)
{
  srand(time(NULL));

  const int NUM_SHOPS = 5;
  const int NUM_CUSTOMERS = 3;

  shop = malloc(NUM_SHOPS * sizeof(int));
  customers = malloc(NUM_CUSTOMERS * sizeof(int));
  finish_customers = malloc(NUM_CUSTOMERS * sizeof(int));
  customer_threads = malloc(NUM_CUSTOMERS * sizeof(pthread_t));

  for (int i = 0; i < NUM_SHOPS; i++)
  {
    shop[i] = INITIAL_STOCK + random_offset(1000);
  }

  for (int i = 0; i < NUM_CUSTOMERS; i++)
  {
    customers[i] = 0;
    finish_customers[i] = 0;
    int *index = malloc(sizeof(int));
    *index = i;
    pthread_create(&customer_threads[i], NULL, customer, index);
  }

  pthread_create(&loader_thread, NULL, loader, NULL);

  for (int i = 0; i < NUM_CUSTOMERS; i++)
  {
    pthread_join(customer_threads[i], NULL);
  }

  pthread_join(loader_thread, NULL);

  free(shop);
  free(customers);
  free(finish_customers);
  free(customer_threads);

  return 0;
}
