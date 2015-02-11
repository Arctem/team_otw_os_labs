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
  int status = 0;
  char *prompt = "totw> ";
  FILE *file = stdin;
  char **commands = NULL;
  int cmd_count = 0;
  int *pids = NULL;

  int error = 0; /* The value execvp returns if invalid command */

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
      if(pids[i] == 0) {
	/* This means we are in a child process and should terminate. */
	should_run = 0;
	break;
      }
    }

    for(i = 0; i < cmd_count; i++) {
      waitpid(pids[i], &status, 0);
      if(status != NULL) {
	fprintf(stderr, "Error %d encountered while waiting on PID %d.\n",
		status, pids[i]);
      }
    }

    
    free(in);
  }

  free(commands);
  free(pids);

  fclose(file);
  exit(0);
}

/* Runs a command and returns the PID of the new process. */
int run_cmd(char *cmd) {
  /* Parse args */
    args[0] = strtok(in, " ");
    
    for(i = 1; (args[i] = strtok(NULL, " ")) != NULL; i++)
      ;

    if((pid = fork()) != 0) {
      if(wait(&status) != pid)
	;
    } else {
      /* Check if there is a semicolon in the arguments, before executing the arguments */
      /* If so, we want to execute the arguments after the semicolon separately */

      //How many arguments are there?
      while(args[arguments] != NULL) {
	arguments++;
      }

      /* Now we can find where a semicolon is */
      for(i = 0; i < arguments; i++){
	if(strcmp(args[i], ";") == 0){
	  /* Hey we found a semicolon */
	  semis++;
	  
	  /* Everything from next_arg to i - 1 is a (hopefully) valid command */
	  /* Store it all into short list */
	  int k = 0;
	  for(j = next_arg; j < i; j++) {
	    short_list[k] = args[j];
	    k++;
	  }
	  short_list[k] = NULL;
	  /* Let's use a fork for concurrency */
	  pid_t cmd = fork();
	  if(cmd >= 0) {
	    if(cmd == 0){
	      error = execvp(short_list[0], short_list);
	      error = is_error(error);
	      exit(0);
	    }
	  }
	  else {
	    perror("Forking failed.\n");
	  }
	  
	  /* Set the next argument counter */
	  next_arg = i + 1;
	  /* printf("i:%d\n", i); */
	}
      }
      if(semis == 0) {
	error = execvp(args[0], args);
	error = is_error(error);

	should_run = 0;
      } else {
	/* Account for the last argument */
	int k = 0;
	for(j = next_arg; j < arguments; j++) {
	  short_list[k] = args[j];
	  k++;
	}
	short_list[k] = NULL;
	error = execvp(short_list[0], short_list);
	error = is_error(error);

	should_run = 0;
      }
    }
    /**
     *After reading user input, the steps are:
     *(1) fork a child process using fork()
     *(2) the child process will invoke execvp()
     *(3) if command include &, parent will invoke wait()
     */
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
