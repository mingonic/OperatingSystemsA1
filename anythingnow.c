#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>

#define CMD_HISTORY_SIZE 10

static char * cmd_history[CMD_HISTORY_SIZE];
static int cmd_history_count = 0;

#define MAX_LINE 80 /* The maximum length command */

static void exec_cmd(const char * line)
{
  char * CMD = strdup(line);
  char *params[10];
  int argc = 0;
  while(params[argc-1] != NULL){
    params[argc++] = strtok(NULL, " ");
  }
  argc--;
  int background = 0;
  if(strcmp(params[argc-1], "&") == 0){
    background = 1;
    params[--argc] = NULL;
  }
  int fd[2] = {-1, -1};
  while(argc >= 3){
  if(strcmp(params[argc-2], ">") == 0){ // output
    fd[1] = open(params[argc-1], O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR| S_IWUSR | S_IRGRP|S_IWGRP);
    if(fd[1] == -1){
      perror("open");
      free(CMD);
      return;
    }
    params[argc-2] = NULL;
    argc -= 2;
  }else if(strcmp(params[argc-2], "<") == 0){ // input
    fd[0] = open(params[argc-1], O_RDONLY);
    if(fd[0] == -1){
      perror("open");
      free(CMD);
      return;
    }
    params[argc-2] = NULL;
    argc -= 2;
    }else{
      break;
    }
  }
  int status;
  pid_t pid = fork();
  switch(pid){
  case -1:
  perror("fork");
  break;
  case 0: // child
  if(fd[0] != -1){
    if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){
      perror("dup2");
      exit(1);
    }
  }
  if(fd[1] != -1){
    if(dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO){
      perror("dup2");
      exit(1);
    }
  }
  execvp(params[0], params);
  perror("execvp");
  exit(0);
  default: // parent
  close(fd[0]);close(fd[1]);
  if(!background)
    waitpid(pid, &status, 0);
    break;
  }
  free(CMD);
}


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

static void add_to_history(const char * cmd)
{
  if(cmd_history_count == (CMD_HISTORY_SIZE-1)){
    int i;
    free(cmd_history[0]);
    for(i=1; i < cmd_history_count; i++)
      cmd_history[i-1] = cmd_history[i];
    cmd_history_count--;
  }
  cmd_history[cmd_history_count++] = strdup(cmd);
}


static void run_from_history(const char * cmd)
{
  int index = 0;
  if(cmd_history_count == 0){
    printf("No commands in history\n");
    return ;
  }
  if(cmd[1] == '!')
    index = cmd_history_count-1;
  else{
    index = atoi(&cmd[1]) - 1;
    if((index < 0) || (index > cmd_history_count)){
      fprintf(stderr, "No such command in history.\n");
    return;
    }
  }
  printf("%s\n", cmd);
  exec_cmd(cmd_history[index]);
}


static void list_history()
{
  int i;
  for(i=cmd_history_count-1; i >=0 ; i--){
    printf("%i %s\n", i+1, cmd_history[i]);
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
        char *temp;
        int i = 0;

        temp = scan(temp);
        token = strtok(temp, " ");

        if (strcmp(token,"exit") == 0) {
            should_run = 0;
        }
      }else if(strcmp(token, "history") == 0){
          list_history();
        }else if(line[0] == '!'){
          run_from_history(token);
        }
        else {
            add_to_history(token);
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
