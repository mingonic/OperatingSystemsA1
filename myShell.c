#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LINE 80 /* The maximum length command */


//the function    https://stackoverflow.com/questions/38211864/dynamically-read-user-input-strings-in-c
char* scan(char *string)
{
    int c; //as getchar() returns `int`
    string = malloc(sizeof(char)); //allocating memory

    string[0]='\0';

    for(int i=0; i<41 && (c=getchar())!='\n' && c != EOF ; i++)
    {
        string = realloc(string, (i+2)*sizeof(char)); //reallocating memory
        string[i] = (char) c; //type casting `int` to `char`
        string[i+1] = '\0'; //inserting null character at the end
    }

    return string;
}

int main (void)
{
    char *args[MAX_LINE/2 +1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */

    while (should_run) {
        printf("CSCI3120>");
        fflush(stdout);

        char *token;
        char *temp;
        int i = 0;
        
        temp = scan(temp);
        token = strtok(temp, " ");

        if (strcmp(token,"exit") == 0) {
            should_run = 0;
        }
        else {
            while (token != NULL)
            {
                // printf("%s\n", token);
                args[i] = token;
                token = strtok (NULL, " ");
                i++;
            }
            //set last value in array to null
            args[i] = NULL;
            i++;
        }

        // // code to print out stuff
        // int j = 0;
        // while (j < i) {
        //     printf("%s\n", args[j]);
        //     j++;
        // }
        // printf("%s\n", args[i-2]);

        pid_t pid;

        pid = fork();

        if (pid == 0) // Child process
        {
            execvp(args[0], args);
        }
        else if (pid > 0) // Parent process
        {
            if (strcmp(args[i-2], "&") == 0)
                wait(NULL);
        }
        free(temp);

        /**
            *After reading user input, the steps are:
            *(1) fork a child process using fork()
            *(2) the child process will invoke execvp()
            *(3) if command included &, parent will invoke wait()
            */
   }
   return 0;
}