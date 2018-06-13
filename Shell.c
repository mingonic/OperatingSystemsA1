#include <stdio.h>
#include <unistd.h>
#include<string.h>

#define MAX_LINE 80 /* The maximum length command */


// void execute(char **argv)
// {
//      pid_t pid;
//      int    status;

//      if ((pid = fork()) < 0) {    
//           printf("*** ERROR: forking child process failed\n");
//           return;
//      }
//      else if (pid == 0) {         
//           if (execvp(*argv, argv) < 0) {   
//                printf("*** ERROR: exec failed\n");
//                return;
//           }
//      }
//      else {                               
//           while (wait(&status) != pid)     
//                ;
//      }
// }


int main (void)
{
     char *args[MAX_LINE/2 +1]; /* command line arguments */
     int should_run = 1; /* flag to determine when to exit program */

     while (should_run) {
          printf("CSCI3120>");

          fflush(stdout);
          char temp[MAX_LINE/2 +1];
          char *token;
          int i = 0;

          scanf("%s\n", temp);
          if (strcmp(temp,"exit") == 0) {
               should_run = 0;
          }
          else {
            token = strtok((char *)temp, " ");
            while( token != NULL ){
                //printf("------------------\n");
                args[i] = token;
                i = i + 1;
                // printf( "%s\n", token);
                token = strtok(NULL, " ");
            }
            int j = 0;
            while (j < i){
                printf(args[j]);
                j = j + 1;
            }
            printf("\n");

            /**
             *After reading user input, the steps are:
            *(1) fork a child process using fork()
            *(2) the child process will invoke execvp()
            *(3) if command included &, parent will invoke wait()
            */
            //  execute(args);

          }
     }
     return 0;
}




