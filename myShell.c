#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LINE 80 /* The maximum length command */
#define HISTORY_SIZE 11

static char * history[HISTORY_SIZE];
static int count = 0;

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

static void add_to_histry(const char * cmd)
{
  if(count == (HISTORY_SIZE-1))
  {
    int i;
    free(history[0]);
    for(i=1; i < count; i++)
      history[i-1] = history[i];
    count--;
  }
  history[count++] = strdup(cmd);
}

static void list_history()
{
  int i;
  for(i = count-1; i >=0; i--){
    printf("%i %s\n", i+1, history[i]);
  }
}

int main (void)
{
    char *args[MAX_LINE/2 +1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */

    while (should_run) {
        printf("CSCI3120>");
        fflush(stdout);

        char *token;
        char *s1;
        char *s2;
        int flag = 1;
        int i = 0;
        
        s1 = scan(s1);
        s2 = strdup(s1);
        token = strtok(s1, " ");

        if (strcmp(token,"exit") == 0) {
            should_run = 0;
        }
        else if(strcmp(token,"history") ==0) {
            list_history();
        }
        else {
            add_to_histry(s2);
            while (token != NULL)
            {
                // printf("%s\n", token);
                args[i] = token;
                token = strtok (NULL, " ");
                i++;
            }
            // Checks whether it contains ampersand. If so, changes flag value and replaces ampersand with NULL to mark end
            if (strcmp(args[i-1], "&") == 0) {
                flag = 0;
                //set last value in array to null
                args[i-1] = NULL;
            } else {
                //set last value in array to null
                args[i] = NULL;
            }

            pid_t pid;

            pid = fork();

            if (pid == 0) // Child process
            {
                execvp(args[0], args);
            }
            else if (pid > 0) // Parent process
            {
                if (flag)
                    wait(NULL);
            }
        }

        // // code to print out stuff
        // int j = 0;
        // while (j < i) {
        //     printf("%s\n", args[j]);
        //     j++;
        // }
        // printf("%s\n", args[i-2]);

        free(s1);
        free(s2);
        /**
            *After reading user input, the steps are:
            *(1) fork a child process using fork()
            *(2) the child process will invoke execvp()
            *(3) if command included &, parent will invoke wait()
            */
   }
   return 0;
}

