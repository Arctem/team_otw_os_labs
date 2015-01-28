/*  Russell White
 *  CSE113
 *  11/8/11
 *  Lab 9
 *  Section 1
 */

#ifndef __LAB_STRINGS
#define __LAB_STRINGS

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

char* get_string(); //Mallocs new string.
char* get_input(char* prompt);  //Gets input.
char* fget_input(FILE* fp); //Gets next line from a file.

#endif
