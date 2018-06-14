#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
#define MAX_LINE 80 /* The maximum length command */
#define HISTORY_SIZE 11 /* THe maximum length of history is 10. It is set to 11 to account for the 0 */

static char * history[HISTORY_SIZE]; /* All commands stored in history */
static int count = 0; /* Keeps track of every command that has gone through history */

//Custom scan function that dynamically allocates memory based on user input
char* scan(char *s)
{
    int ch;
    // allocates memory to string and setting null character
    s = malloc(sizeof(char));
    s[0]='\0';

    // loops running until end of file reached or new line
    for(int i=0; i<41 && (ch=getchar())!='\n' && ch != EOF ; i++)
    {
        // reallocating memory, making the int a char and setting null char at the end
        s = realloc(s, (i+2)*sizeof(char)); 
        s[i] = (char) ch; 
        s[i+1] = '\0';
    }

    return s;
}

/* Function that lists last 10 items in history when called */
static void list_history()
{
    int i;
    int j;
    int left;
    if (count < 10) {
        left = 10-count;
    }
    else {
        left = 0;
    }
    i = count-1;

    for(j = left; j < 10; j++){
        printf("%i %s\n", i+1, history[i]);
        i--;
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
        char *position;
        char *s1;
        char *s2;
        char *s3;
        char h;
        int flag = 1;
        int error = 1;
        int i = 0;
        
        // Scanning user input into string s1 using custom scan function
        s1 = scan(s1);
        // Making copies of string s1 for later usage
        s2 = strdup(s1);
        s3 = strdup(s1);
        // Getting first char of the inputted string, used to check in 2nd technique below
        h = s3[0];
        // Tokenizing string, seperating by whitespace
        token = strtok(s1, " "); 

        /* If user enters exit, then program should not run */
        if (strcmp(token,"exit") == 0) {
            should_run = 0;
        }
        /* If user enters history, then it should call list history function */
        else if(strcmp(token,"history") == 0) {
            //Error check
            if (count == 0) {
                printf("%s\n", "No commands in history");
                error = 0;
            }
            else {
                list_history();
            }
        }
        else {
            /* 1st technique to retrieve and execute most recent command */
            if (strcmp(token, "!!") == 0) {
                //Error check
                if (count == 0) {
                    printf("%s\n", "No commands in history");
                    error = 0;
                } else {
                    // Retrives most recent command and tokenizes it before executing 
                    s1 = strdup(history[count-1]);
                    s2 = strdup(s1);
                    printf("%s\n", s1);
                    token = strtok(s1, " ");
                }
            }
            /* 2nd technique to retrieve and execute a specified command from the history */
            else {
                if (h == '!') {
                    position = strtok(s3, "!");
                    // Using atoi function to convert from string to integer
                    int num = atoi(position);
                    // Retrives a specific command within bounds of history and tokenizes it before executing
                    if (num <= count && num > 0 && num > count-10) {
                        s1 = strdup(history[num-1]);
                        s2 = strdup(s1);
                        printf("%s\n", s1);
                        token = strtok(s1, " ");
                    } else {
                        //Error check
                        printf("%s\n", "No such command in history");
                        error = 0;
                    }
                }
            }

            /* Error handling, this section would not execute with error */
            if (error) {
                // Add commands to history
                history[count++] = strdup(s2);
                /* adds next tokens in commands to the args array */
                while (token != NULL)
                {
                    args[i] = token;
                    token = strtok (NULL, " ");
                    i++;
                }
                /* Checks whether it contains ampersand.*/
                /* If so, changes flag value and replaces ampersand with NULL to mark end*/
                if (strcmp(args[i-1], "&") == 0) {
                    flag = 0;
                    //set last value in array to null
                    args[i-1] = NULL;
                } else {
                    //set last value in array to null
                    args[i] = NULL;
                }

                /* Process creation and execution */
                pid_t pid;
                pid = fork();

                if (pid == 0) // Child process
                {
                    execvp(args[0], args);
                }
                else if (pid > 0) // Parent process
                {
                    //flag to check if ampersand is in the command
                    if (flag)
                        wait(NULL);
                }
            }
        }

        // Deallocating memory
        free(s1);
        free(s2);
        free(s3);
   }
   return 0;
}

