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
  // FIX - don't add incorrect commands
  int flag = 0;
  const char* home = getenv("HOME");
  char filepath[MAXLINE];
  snprintf(filepath, sizeof(filepath), "%s/documentation/commands.txt", home);
  FILE* infile = fopen(filepath, "a+");   // open for reading and appending
  char line[MAXLINE];
  while (fgets(line, sizeof(line), infile) != NULL) {
    char command[100];
    sscanf(line, "%s", command);
    if (strcmp(argv[1], "sudo")) {   // check for 'sudo'
      if (!strcmp(argv[1], command)) {   // command exists
	  printf("Command already exists. Exiting program...\n");
	  fclose(infile);
	  exit(0);
	}
    } else {
      flag = 1;
      if (!strcmp(argv[2], command)) {   // command exists
	printf("Command already exists. Exiting program...\n");
	fclose(infile);
	exit(0);
      }
    }
  }
    
  printf("\n");

  // Add command to file
  if (!flag) {
    fprintf(infile, "%s - ", argv[1]);
  } else {
    fprintf(infile, "%s - ", argv[2]);
  }

  // Prompt user for explanation of command and add to file
  printf("Why did you use this command?\n");
  memset(line, 0, sizeof(line));
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
  fprintf(infile, ")\n");
     
  fclose(infile);
  
  return 0;
}
