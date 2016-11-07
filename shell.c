#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

void prompt(int *stat) {
  if(stat != NULL) printf("%d", *stat);
  printf("> ");
}

void display_command(char **argv) {
  char **current = argv;
  printf("Command: ");
  while(*current != NULL) {
    printf("%s ", *current);
    current++;
  }
  printf("\n");
}

void chomp(char *line) {
  char *pos = NULL;
  char *invalid = "\n\t\r";

  while(*invalid != '\0') {
    if ((pos = strchr(line, *invalid)) != NULL)
      *pos = '\0';

    invalid++;
  }
}

int parse(char *line, char **argv) {
  chomp(line);

  char *token = NULL;
  int argc = 0;

  while(argc < ARG_MAX && (token = strsep(&line, " ")) != NULL) {
    if(strcmp(token, "") == 0) continue;

    argv[argc] = token;
    argc++;
  }
  argv[argc] = NULL;

  return argc;
}

void run(char **argv, int* stat) {
  pid_t pid = fork();

  if(pid == 0) {
    if(execvp(*argv, argv) == -1) perror("execvp");
  } else if(pid <= -1) {
    perror("fork");
  } else {
    waitpid(pid, stat, 0);
  }
}

int main() {
  char *argv[ARG_MAX] = {NULL};
  int argc = 0;
  char *line = NULL;
  size_t linelen = 0;
  int stat = 0;

  for(;;) {
    prompt(&stat);

    getline(&line, &linelen, stdin);
    argc = parse(line, argv);

    if(argc == 0) continue;
    if(strcmp(argv[0], "exit") == 0) break;

    display_command(argv);
    run(argv, &stat);
  }

  free(line);
  return 0;
}

