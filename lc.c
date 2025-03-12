#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 4096

int main(int argc, char** argv) {

  // Command-line error checking
  if (argc < 2) {
    fprintf(stderr, "Must provide a command\n");
    exit(EXIT_FAILURE);
  }

  // Fork child process to execute command
  pid_t pid = fork();
  if (pid == 0) {
    // CHILD PROCESS
    if (execvp(argv[1], &argv[1]) == -1) {
      perror("Cannot execute");
      exit(EXIT_FAILURE);
    }
  } else {
    // PARENT PROCESS
    int wstatus;
    wait(&wstatus);
  }

  // Check if command already stored in commands.txt
  FILE* infile = fopen("commands.txt", "a+");   // open for reading and appending
  char line[MAXLINE];
  while (fgets(line, sizeof(line), infile) != NULL) {
    char command[100];
    sscanf(line, "%s", command);
    if (!strcmp(argv[1], command)) {   // command exists
      printf("Command already exists. Exiting program...\n");
      fclose(infile);
      exit(0);
    }
  }
    
  printf("\n");

  // Add command to file
  fprintf(infile, "%s - ", argv[1]);

  // Prompt user for explanation of command and add to file
  printf("Why did you use this command?\n");
  fgets(line, sizeof(line), stdin);
  size_t len = strlen(line);
  if (len > 0 && line[len - 1] == '\n') {
    line[len - 1] = ' ';   // replace newline with space to set up for appending example usage
  }
  fprintf(infile, "%s", line);

  // Add your usage of command to file as an example
  fprintf(infile, "(Usage: ");
  for (int i = 1; i < argc; i++) {
    if (i == argc - 1) {
      fprintf(infile, "%s", argv[i]);
    } else {
      fprintf(infile, "%s ", argv[i]);
    }
  }
  fprintf(infile, ")");
     
  fclose(infile);
  
  return 0;
}
