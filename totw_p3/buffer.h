/*****************************************************
 * Filename: buffer.h
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

#ifndef __BUFFER_H_
#define __BUFFER_H_

#define BUFFER_SIZE 10

typedef struct _thread_buffer {
  int stack;
  int *data;
} buffer;

/* Functions to add/remove from the buffer */
/* Will respect the setting of stack inside struct */
void add_to(buffer *buff, int value);
int pop(buffer *buff);

int is_empty(buffer *buff);
int is_full(buffer *buff);

void print_buffer(buffer *buff);
#endif
