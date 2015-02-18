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

#define BUFFER_SIZE 10


int main(int argc, char *argv[]) {
  int i;
  
  /* 1. Get command line arguments */
  int num_producers = atoi(argv[1]);
  int num_consumers = atoi(argv[2]);
  int filo = atoi(argv[3]);

  /* 2. Initialize buffer entries with -1 */
  int *buffer = malloc(BUFFER_SIZE * sizeof(int));
  for(i = 0; i < BUFFER_SIZE; i++)
    buffer[i] = -1;

  /* 3. Create producer thread(s) */
  /* 4. Create consumer thread(s) */
  /* 5. Sleep 300 seconds */
  /* 6. Exit */
  free(buffer);

  return 0;
}
