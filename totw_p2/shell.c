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
 * Assignment: Lab Project #2
 * Assigned: January 29, 2015
 * Due: February 11, 2015
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>

void trim(char *str);
char* get_string();
char* fget_input(FILE* fp); /* Gets next line from a file. */
int run_cmd(char *cmd);
int is_error(int err); /* Prints out error message, changes error back to 0 */

int main(int argc, char *argv[]) {
  int should_run = 1; /* flag to determine when to exit */
  char *in = NULL;
  char *next_cmd = NULL;
  int i = 0;
  //int status = 0;
  char *prompt = "totw> ";
  FILE *file = stdin;
  char **commands = NULL;
  int cmd_count = 0;
  int *pids = NULL;

  if(argc == 2) {
    file = fopen(argv[1], "r");
    if(file == 0) {
      fprintf(stderr, "Could not open %s.\n", argv[1]);
      return -1;
    }
  } else if(argc > 2) {
    fprintf(stderr, "Too many arguments.\n");
    return -1;
  }

  while(should_run) {
    if(file == stdin) {
      in = readline(prompt);
    }
    else
      in = fget_input(file);
    

    if(in == NULL) {
      should_run = 0;
      continue;
    }

    /* Check for exit. */
    if((strncmp(in, "exit", 4) == 0) || (strncmp(in, "quit", 4) == 0)) {
      should_run = 0;
      free(in);
      continue;
    }


    /* Split into multiple commands by semicolons */
    cmd_count = 0;
    next_cmd = strtok(in, ";");

    while(next_cmd != NULL) {
      cmd_count++;
      commands = realloc(commands, cmd_count * sizeof(char*));
      commands[cmd_count - 1] = next_cmd;
      next_cmd = strtok(NULL, ";");
    }

    pids = malloc(cmd_count * sizeof(int));

    for(i = 0; i < cmd_count; i++) {
      pids[i] = run_cmd(commands[i]);
    }

    
    free(in);
  }

  free(commands);

  fclose(file);
  exit(0);
}

/* Runs a command and returns the PID of the new process. */
int run_cmd(char *cmd) {
  int error = 0; /* The value execvp returns if invalid command */

  /* Let's use a fork for concurrency */
  pid_t forker = fork();
  if(forker >= 0) {
    if(forker == 0){
      error = execvp(cmd[0], cmd);
      error = is_error(error);
    }
  }  else {
    perror("Forking failed.\n");
  }

  /**
  *After reading user input, the steps are:
  *(1) fork a child process using fork()
  *(2) the child process will invoke execvp()
  *(3) if command include &, parent will invoke wait()
  */

  return 0;
}

//Returns a pointer to a new string consisting of a null terminator.
char* get_string() {
  char* str = malloc(1);
  if(!str) {
    fprintf(stderr, "malloc failed to allocate memory\n");
    exit(-1);
  }
  str[0] = '\0';
  return str;
}

char* fget_input(FILE *fp) {
  char* str = get_string();   /* String to return. */
  char input = 0; /* Stores user input character by character. */
  int length = 1; /* Size of one to start: just '\0' */
    
  do {
    input = fgetc(fp);
        
    if(input == '\n' || input == '\0')
      break;
        
    if(input == EOF) {
      free(str);
      return NULL;
    }
            
    str = realloc(str, ++length);
    if (!str) {
      fprintf(stderr, "realloc failed to allocate memory\n");
      exit(-1);
    }
        
    str[length - 2] = input; /* Set second to last character to the new input. */
  } while(1);
    
  str[length - 1] = '\0';  /* Set the last character to be a null terminator. */
    
  return str;
}

int is_error(int err) {
  if(err < 0) {
    perror("Invalid command");
  }
  return (0);
}
