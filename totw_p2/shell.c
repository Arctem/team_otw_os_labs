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

//For the threads
#include <pthread.h>

void trim(char *str);
char* get_string();
char* fget_input(FILE* fp); //Gets next line from a file.

int isError(int err); //Prints out error message, changes error back to 0

int main(int argc, char *argv[]) {
  char *args[80];
  int should_run = 1; /* flag to determine when to exit */
  char *in = NULL;
  int i = 0;
  int j = 0;
  int status = 0;
  pid_t pid = 0;
  char *prompt = "totw> ";
  FILE *file = stdin;

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

  int arguments = 0; //Number of arguments we have
  int semis = 0; //Number of semicolons we have
  int next_arg = 0; //Which argument we start at next
  char *short_list[80]; //In the event we have semicolons, this keeps a short list of it
  int error = 0; //The value execvp returns if invalid command

  while(should_run) {
    //in = get_input(prompt);
    if(file == stdin) {
      in = readline(prompt);
    }
    else
      in = fget_input(file);
    

    if(in == NULL) {
      should_run = 0;
      continue;
    }
        
    trim(in);

    /* Check for exit. */
    if((strncmp(in, "exit", 4) == 0) || (strncmp(in, "quit", 4) == 0)) {
      should_run = 0;
      free(in);
      continue;
    }
      
    /* Parse args */
    args[0] = strtok(in, " ");
    
    for(i = 1; (args[i] = strtok(NULL, " ")) != NULL; i++)
      ;

    if((pid = fork()) != 0) {
      if(wait(&status) != pid)
	;
    } else {
      //Check if there is a semicolon in the arguments, before executing the arguments
      //If so, we want to execute the arguments after the semicolon separately

      //How many arguments are there?
      while(args[arguments] != NULL) {
	arguments++;
      }

      //Now we can find where a semicolon is
      for(i = 0; i < arguments; i++){
	if(strcmp(args[i], ";") == 0){
	  //Hey we found a semicolon
	  semis++;
	  
	  //Everything from next_arg to i - 1 is a (hopefully) valid command
	  //Store it all into short list
	  int k = 0;
	  for(j = next_arg; j < i; j++) {
	    short_list[k] = args[j];
	    k++;
	  }
	  short_list[k] = NULL;
	  //Let's use a fork for concurrency
	  pid_t cmd = fork();
	  if(cmd >= 0) {
	    if(cmd == 0){
	      error = execvp(short_list[0], short_list);
	      error = isError(error);
	      exit(0);
	    }
	  }
	  else {
	    perror("Forking failed.\n");
	  }
	  
	  //Set the next argument counter
	  next_arg = i + 1;
	  //printf("i:%d\n", i);
	}
      }
      if(semis == 0) {
	error = execvp(args[0], args);
	error = isError(error);
	exit(0);
      } else {
	//Account for the last argument
	int k = 0;
	for(j = next_arg; j < arguments; j++) {
	  short_list[k] = args[j];
	  k++;
	}
	short_list[k] = NULL;
	error = execvp(short_list[0], short_list);
	error = isError(error);
	exit(0);
      }
    }
    /**
     *After reading user input, the steps are:
     *(1) fork a child process using fork()
     *(2) the child process will invoke execvp()
     *(3) if command include &, parent will invoke wait()
     */
    free(in);
  }

  fclose(file);
  exit(0);
}

/* Removes leading and trailing spaces. */
void trim(char *str) {
  if(str == NULL)
    return;
  
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
  char* str = get_string();   //String to return.
  char input = 0; //Stores user input character by character.
  int length = 1; //Size of one to start: just '\0'
    
  do {
    input = fgetc(fp);
        
    //End the loop if user ends the input.
    if(input == '\n' || input == '\0')
      break;
        
    if(input == EOF) {
      free(str);
      return NULL;
    }
            
    str = realloc(str, ++length); //Increases size by 1.
    if (!str) {
      fprintf(stderr, "realloc failed to allocate memory\n");
      exit(-1);
    }
        
    str[length - 2] = input; //Set second to last character to the new input.
  } while(1);
    
  str[length - 1] = '\0';  //Set the last character to be a null terminator.
    
  return str;
}

int isError(int err) {
  if(err < 0) {
    perror("Invalid command.");
  }
  return (0);
}
