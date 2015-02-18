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
 * Assigned: February 11, 2015
 * Due: February 25, 2015
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "buffer.h"

typedef struct _thread_data {
  int thread_num;
  buffer *buff;
  sem_t *semaphore;
  int stack;
} thread_data;

void *producer_func(void *data);
void *consumer_func(void *data);

int main(int argc, char *argv[]) {
  int i;
  pthread_t *producers;
  pthread_t *consumers;
  sem_t *semaphore;

  if(argc != 4) {
    perror("Usage: ./p3 producers consumers FIFO/FILO");
    return -1;
  }
  
  /* 1. Get command line arguments */
  int num_producers = atoi(argv[1]);
  int num_consumers = atoi(argv[2]);
  int stack = atoi(argv[3]);

  /* 2. Initialize buffer entries with -1 */
  buffer *buff = malloc(sizeof(buffer));
  buff->data = malloc(BUFFER_SIZE * sizeof(int));
  buff->stack = stack;
  for(i = 0; i < BUFFER_SIZE; i++)
    buff->data[i] = -1;

  semaphore = malloc(sizeof(sem_t));
  sem_init(semaphore, 0, 1);

  /* 3. Create producer thread(s) */
  producers = malloc(num_producers * sizeof(pthread_t));
  for(i = 0; i < num_producers; i++) {
    thread_data *data = malloc(sizeof(thread_data));
    data->thread_num = i;
    data->buff = buff;
    data->semaphore = semaphore;
    data->stack = stack;
    pthread_create(&producers[i], NULL, producer_func, data);
  }
  
  /* 4. Create consumer thread(s) */
  consumers = malloc(num_consumers * sizeof(pthread_t));
  for(i = 0; i < num_consumers; i++) {
    thread_data *data = malloc(sizeof(thread_data));
    data->thread_num = i;
    data->buff = buff;
    data->semaphore = semaphore;
    data->stack = stack;
    pthread_create(&consumers[i], NULL, consumer_func, data);
  }
  
  /* 5. Sleep 300 seconds */
  sleep(20);
  
  /* 6. Exit */
  free(producers);
  free(consumers);
  free(semaphore);
  free(buff);

  return 0;
}

void *producer_func(void *data) {
  int thread_num = ((thread_data*) data)->thread_num;
  buffer *buff = ((thread_data*) data)->buff;
  sem_t *semaphore = ((thread_data*) data)->semaphore;
  printf("Producer %d starting.\n", thread_num);

  while(1) {
    usleep(rand() % 1000000); /* usleep takes input in microseconds */
    sem_wait(semaphore);
    printf("Producer %d locked it.\n", thread_num);
    usleep(rand() % 1000000); /* usleep takes input in microseconds */
    printf("Producer %d done waiting.\n", thread_num);

    /*Check if buffer is full before adding something*/
    if(){
      /*add_to(buff, rand());*/
    }
    sem_post(semaphore);
  }

  free(data);
  return NULL;
}

void *consumer_func(void *data) {
  int thread_num = ((thread_data*) data)->thread_num;
  buffer *buff = ((thread_data*) data)->buff;
  sem_t *semaphore = ((thread_data*) data)->semaphore;
  printf("Consumer %d starting.\n", thread_num);

  while(1) {
    usleep(rand() % 1000000); /* usleep takes input in microseconds */
    sem_wait(semaphore);
    printf("Consumer %d locked it.\n", thread_num);
    usleep(rand() % 1000000); /* usleep takes input in microseconds */
    printf("Consumer %d done waiting.\n", thread_num);

    /*Check if buffer is empty before grabbing something*/
    if(){
	/*pop(buff);*/
    }
    sem_post(semaphore);
  }

  free(data);
  return NULL;
}
