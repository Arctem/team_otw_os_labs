/*****************************************************
 * Filename: p3.c
 *
 * Description: 
 *
 * Team: Omega Tiger Woods
 * Authors: Andrew Baker and Russell White
 * Class: CSE 325
 * Instructor: Zheng
 * Assignment: Lab Project #3
 * Assigned: January 11, 2015
 * Due: February 25, 2015
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

void *producer_func(void *data);
void *consumer_func(void *data);

typedef struct _thread_data {
  int thread_num;
  int *buffer;
  sem_t *semaphore;
} thread_data;

int main(int argc, char *argv[]) {
  int i;
  pthread_t *producers;
  pthread_t *consumers;
  sem_t *semaphore;
  
  /* 1. Get command line arguments */
  int num_producers = atoi(argv[1]);
  int num_consumers = atoi(argv[2]);
  int filo = atoi(argv[3]);

  /* 2. Initialize buffer entries with -1 */
  int *buffer = malloc(BUFFER_SIZE * sizeof(int));
  for(i = 0; i < BUFFER_SIZE; i++)
    buffer[i] = -1;

  semaphore = malloc(sizeof(sem_t));
  sem_init(semaphore, 0, 0);

  /* 3. Create producer thread(s) */
  producers = malloc(num_producers * sizeof(pthread_t));
  for(i = 0; i < num_producers; i++) {
    thread_data *data = malloc(sizeof(thread_data));
    data->thread_num = i;
    data->buffer = buffer;
    data->semaphore = semaphore;
    pthread_create(&producers[i], NULL, producer_func, data);
  }
  
  /* 4. Create consumer thread(s) */
  consumers = malloc(num_consumers * sizeof(pthread_t));
  for(i = 0; i < num_consumers; i++) {
    thread_data *data = malloc(sizeof(thread_data));
    data->thread_num = i;
    data->buffer = buffer;
    data->semaphore = semaphore;
    pthread_create(&consumers[i], NULL, consumer_func, data);
  }
  
  /* 5. Sleep 300 seconds */
  sleep(20);
  
  /* 6. Exit */
  free(producers);
  free(consumers);
  free(semaphore);
  free(buffer);

  return 0;
}

void *producer_func(void *data) {
  int thread_num = ((thread_data*) data)->thread_num;
  int *buffer = ((thread_data*) data)->buffer;
  sem_t *semaphore = ((thread_data*) data)->semaphore;
  printf("Producer %d starting.\n", thread_num);

  while(1) {
    usleep(rand() % 1000000); /* usleep takes input in microseconds */
    printf("Producer %d done waiting.\n", thread_num);
  }

  free(data);
  return NULL;
}

void *consumer_func(void *data) {
  int thread_num = ((thread_data*) data)->thread_num;
  int *buffer = ((thread_data*) data)->buffer;
  sem_t *semaphore = ((thread_data*) data)->semaphore;
  printf("Consumer %d starting.\n", thread_num);

  while(1) {
    usleep(rand() % 1000000); /* usleep takes input in microseconds */
    printf("Consumer %d done waiting.\n", thread_num);
  }

  free(data);
  return NULL;
}
