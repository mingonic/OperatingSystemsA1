#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80 /* The maximum length command */

int main (void)
{
char *args[MAX_LINE/2 +1]; /* command line arguments */
int should_run = 1; /* flag to determine when to exit program */

    while (should_run) {
        printf("CSCI3120>");
        fflush(stdout);

        char *temp;
        char *token;
        int i = 0;

        scanf("%s", &temp);
        token = strtok(&temp, " ");
        while( token != NULL ){
          args[i] = token;
          printf(" %s\n", &token);

          token = strtok(NULL, " ");
          i = i + 1;

        }
        args[i] = NULL;

        int j = 0;
        while (i > j){
            char *word = &args[j];
            printf("%p\n", *word);
            j = j + 1;
        }



        /**
         *After reading user input, the steps are:
         *(1) fork a child process using fork()
         *(2) the child process will invoke execvp()
         *(3) if command included &, parent will invoke wait()
         */
    }
    return 0;
}
