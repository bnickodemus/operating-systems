#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace std;

int fork_pipes (int n, char* cmd[], char* array2[])
{
  int i, fd [2];
  int in = 0;

  for (i = 0; i < n - 1; i++) {
      pipe (fd);

      switch(fork()) {
        case -1: {
          perror("BAD FORK"); // fork might fail
          exit(-1);
        }
        case 0: {
            if (in != 0) { // in represents the write end of the pipe from previous pipe
                int num = dup2 (in, 0); // dup the write end of previous pipe as the current read
                if (num == -1) {
                  perror("BAD DUP");
                  exit(-1);
                }
                close (in);
            }
            if (fd[1] != 1) {
                int num = dup2 (fd[1], 1); // dup the write end of the pipe
                if (num == -1) {
                  perror("BAD DUP");
                  exit(-1);
                }
                close (fd[1]);
            }
            cmd += i; // look at the next command
            printf("arr2 %s\n",array2[i]);
            execvp (array2[i], (char * const *)array2);
          }
        default: {
          close (fd [1]);
          in = fd [0]; // keep the read end of the pipe for the next pipe
        }
    }
  }
  if (in != 0) {
    int num = dup2 (in, 0);
    if (num == -1) {
      perror("BAD DUP"); // dup might fail
      exit(-1);
    }
  }
  execvp (cmd[i], (char * const *)cmd);
  return -1;
}

int main(int argc, char **argv)
{
  if (argc > 1) {
    printf("%s%s",argv[1],"> ");
  }
  else
    printf("my_shell> ");

  char ** array1 = NULL;
  char *line = NULL, *ptr; //, **argvs;
  size_t n = 0;
  int status;
  vector <char *> v;

  // read lines of input from user
  while (getline(&line, &n, stdin) != -1) {

    if(strcmp(line, "exit\n") == 0) // exit when the user enters exit
      exit(0);

    if (argc > 1) {
      printf("%s%s",argv[1],"> ");
    }
    else
      printf("my_shell> ");

    //parse into argv array (null terminated)
    ptr = strtok(line, " \n");
    while (ptr != NULL) {
      v.push_back(ptr);
      ptr = strtok(NULL, " \n");
    }

    array1 = (char **) malloc(sizeof(char *) * (v.size() + 1));

    for (int i = 0; i < v.size(); i++)
    {
      array1[i] = v[i]; // copy all args to array1
    }
    array1[v.size()] = NULL;

    // array2 represents the arguments with redirection parsed out
    char ** array2 = (char **) malloc(sizeof(char *) * (v.size() + 1));
    int counter = 0;
    for (int i = 0; i < v.size(); i++) {
      //printf("ar1: %s\n",array1[i]);
      // if we find a redirection, break
      if (strcmp(v[i],"<") == 0 || strcmp(v[i],">") == 0 || strcmp(v[i],"<<") == 0 || strcmp(v[i],">>") == 0)  {
        array2[i] = NULL;
        break;
      }
      else {
        array2[i] = v[i];
      }
    }

    FILE *file;
    int fd;

    bool redirect = true; // do we have redirections?
    int numPipes = 0;

    switch(fork()) {
      case -1: {
        perror("BAD FORK"); // fork might fail
        exit(-1);
      }
      case 0: {
        for (int i = 0; i < v.size(); i++) { // loop through all args

          if (strcmp(array1[i],"<") == 0) { // we found a <
            file = fopen(array1[i + 1],"r"); // we know the next arg is a file
            if (file == NULL) {
              perror("BAD FOPEN"); // fopen might fail
              exit(-1);
            }
            fd = fileno(file);
            if (fd == -1) {
                perror("BAD FILENO"); // fileno might fail
                exit(-1);
            }
            int num = dup2(fd, 0); // redirect to std in
            if (num == -1) {
              perror("BAD DUP"); // dup might fail
              exit(-1);
            }
            redirect = false;
          }
          else if (strcmp(array1[i],">") == 0) { // we found a >
            //printf("found %s\n",array1[i + 1]);
            file = fopen(array1[i + 1],"w"); // we know the next arg is a file
            if (file == NULL) {
              perror("BAD FOPEN");
              exit(-1);
            }
            fd = fileno(file);
            if (fd == -1) {
                perror("BAD FILENO");
                exit(-1);
            }
            int num = dup2(fd, 1); // redirect to std out
            if (num == -1) {
              perror("BAD DUP");
              exit(-1);
            }
            redirect = false;
          }
          else if (strcmp(array1[i],">>") == 0) { // we found a >>
            //int myfd = open(array1[i + 1], O_WRONLY | O_CREAT | O_APPEND, S_IWUSR | S_IRUSR); // idk why but this didn't work
            file = fopen(array1[i + 1], "a"); // a for append mode, this ain't portable, bummer
            if (file == NULL) {
              perror("BAD FOPEN");
              exit(-1);
            }
            fd = fileno(file);
            if (fd == -1) {
                perror("BAD FILENO");
                exit(-1);
            }
            int num = dup2(fd, 1); // append to std out
            if (num == -1) {
              perror("BAD DUP");
              exit(-1);
            }
            redirect = false;
          }
          else if (strcmp(array1[i],"2>") == 0) { // we found a 2>
            file = fopen(array1[i + 1],"w");
            if (file == NULL) {
              perror("BAD FOPEN");
              exit(-1);
            }
            fd = fileno(file);
            if (fd == -1) {
                perror("BAD FILENO");
                exit(-1);
            }
            int num = dup2(fd, 2); // output to std err
            if (num == -1) {
              perror("BAD DUP");
              exit(-1);
            }
            redirect = false;
          }
          else if (strcmp(array1[i],"|") == 0) {
            numPipes++;
          }
          else {
            // if we are at the end with no redirections or pipes
            if ((i == v.size() - 1) && redirect && (numPipes == 0)) {
                execvp(array1[0], array1); // exec with array1
            }
            // are there pipes to handle?
            if (numPipes != 0) {
              int num = fork_pipes (numPipes, array1, array2);
              if (num == -1)
                perror("fork_pipes exec() failed");
            }
          }
        }
        execvp(array2[0], array2);
        perror("Invalid Command. Try again"); // user might enter a bad command
        //exit(-1); // we don't need to exit here
      }
    default: {
        // wait on child
        wait(&status);
        // clear allocated memory
        v.clear();
        free(array1);
        free(array2);
      }
    }

  } // e-of while input
} // e-of main
