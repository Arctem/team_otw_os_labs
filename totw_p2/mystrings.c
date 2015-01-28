/*  Russell White
 *  CSE113
 *  11/8/11
 *  Lab 9
 *  Section 1
 */

#include "mystrings.h"

//Returns a pointer to a new string consisting of a null terminator.
char* get_string() {
    char* str = malloc(1);  //Char size is 1, so we don't need a sizeof(char)
    if (!str) {
        fprintf(stderr, "malloc failed to allocate memory\n");
        exit(-1);
    }
    str[0] = '\0';
    return str;
}

char* get_input(char* prompt) {
    char* str = get_string();   //String to return.
    char input; //Stores user input character by character.
    int length = 1; //Size of one to start: just '\0'
    
    printf("%s", prompt);   //Prompt the user.
    do {
        input = getchar();
        
        //End the loop if user ends the input.
        if(input == '\n' || input == '\0')
            break;
            
        str = realloc(str, ++length); //Increases size by 1.
        if (!str) {
            fprintf(stderr, "realloc failed to allocate memory\n");
            exit(-1);
        }
        
        str[length - 2] = input; //Set second to last character to the new input.
    } while (TRUE);
    
    str[length - 1] = '\0';  //Set the last character to be a null terminator.
    
    return str;
}

char* fget_input(FILE *fp) {
    char* str = get_string();   //String to return.
    char input; //Stores user input character by character.
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
    } while (TRUE);
    
    str[length - 1] = '\0';  //Set the last character to be a null terminator.
    
    return str;
}
