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

void trim(char *str);
char* get_string();
char* fget_input(FILE* fp); //Gets next line from a file.

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
  int next_arg = 0; //Which argument we start at next
  char *short_list[arg_size]; //In the event we have semicolons, this keeps a short list of it

  while(should_run) {
    //in = get_input(prompt);
    if(file == stdin) {
      fprintf(stdout, "%s", prompt);
    }
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
	  /*
	  short_list[next_arg] = strtok(in, " ");
	  for(j = next_arg + 1; ((j < semi_location) && (short_list[j] = strtok(NULL, " ")) != NULL); j++)
	    ;
	  execvp(short_list[0], short_list); //Execute command up to the semi colon
	  next_arg = semi_location + 1;//Update next command
	  */
	}
      }
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
