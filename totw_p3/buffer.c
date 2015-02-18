/*****************************************************
 * Filename: buffer.c
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

#include "buffer.h"

/* Stack will add to the start of the list, */
/* queue will add to the end. */
void add_to(buffer *buff, int value) {
  int i;
  
  if(buff->stack) {
    for(i = BUFFER_SIZE - 1; i > 0; i++)
      buff->data[i] = buff->data[i - 1];
    buff->data[0] = value;
  } else {
    for(i = 0; buff->data[i] != -1; i++)
      ;
    buff->data[i] = value;
  }
}

int pop(buffer *buff) {
  int i;
  int retval = buff->data[0];
  for(i = 0; i < BUFFER_SIZE - 1; i++)
    buff->data[i] = buff->data[i + 1];
  
  return retval;
}

/* -1 indicates no value, so if we see one it's empty */
int is_empty(buffer *buff) {
  return buff->data[0] == -1;
}

int is_full(buffer *buff) {
  return buff->data[BUFFER_SIZE - 1] != -1;
}
