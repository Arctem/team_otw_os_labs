/*****************************************************
 * Filename: shell.c
 *
 * Description: This program implements a simple CLI
 * similar to bash.
 *
 * Team: Omega Tiger Woods
 * Authors: Andrew Baker and Russell White
 * Class: CSE 325
 * Instructor: Zheng
 * Assignment: Lab Project #1
 * Assigned: January 29, 2015
 * Due: February 11, 2015
 *****************************************************/

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include "mystrings.h"

#define MAX_LINE 80 /* The maximum length command */

void trim(char *str);

int main(void) {
  int arg_size = MAX_LINE / 2 + 1;
  char *args[arg_size]; /* command line arguments */
  int should_run = 1; /* flag to determine when to exit */
  char *in = NULL;
  int i = 0;
  int status = 0;
  pid_t pid = 0;
  int nowait = 0;
  char *prompt = NULL;

  while(should_run) {
    prompt = getcwd(NULL, 0);
    strcat(prompt, "$ ");
    
    in = get_input(prompt);
    free(prompt);
    
    trim(in);
    
    nowait = in[strlen(in) - 1] == '&';
    /* Trim the & */
    if(nowait) {
      in[strlen(in) - 1] = '\0';
      trim(in);
    }

    /* Check for exit. */
    if(strncmp(in, "exit", 4) == 0) {
      should_run = 0;
      free(in);
      continue;
    }
      
    
    /* Parse args */
    args[0] = strtok(in, " ");
    
    for(i = 1; (args[i] = strtok(NULL, " ")) != NULL; i++)
      ;
    //if(args[0] == NULL)
    //args[0] = "";

    if((pid = fork()) != 0) {
      if(!nowait)
	if(wait(&status) != pid)
	  ;
    } else {
      execvp(args[0], args);
      exit(0);
    }
    /**
     *After reading user input, the steps are:
     *(1) fork a child process using fork()
     *(2) the child process will invoke execvp()
     *(3) if command include &, parent will invoke wait()
     */
    free(in);
  }
  exit(0);
}

/* Removes leading and trailing spaces. */
void trim(char *str) {
  char *start = str;
  while(isspace(*start))
    start++;

  if(start != str)
    strcpy(str, start);

  char *end = (str + strlen(str)) - 1;

  while(isspace(*end)) {
    *end = '\0';
    end--;
  }
}
