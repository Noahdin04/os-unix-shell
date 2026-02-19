#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_INPUT 1024
#define MAX_ARGS 64
/* TODO: Implement a safe parser that fills argv[] and returns argc.
Requirements: argv must end with NULL. Handle extra spaces. */
static int parse_command(char *line, char *argv[], int max_args)
{
/* TODO */
return 0;
}
int main(void)
{
char input[MAX_INPUT];
char *argv[MAX_ARGS];
while (1) {
/* TODO: print prompt and flush */
/* TODO: read a line with fgets; handle EOF */
/* TODO: strip newline */
/* TODO: if empty line, continue */
/* TODO: if input == "exit", print message and break */
/* TODO: parse_command(input, argv, MAX_ARGS); if no args,
continue */
pid_t pid = fork();
if (pid < 0) {
perror("fork");
continue;
}
if (pid == 0) {
/* TODO: Child: execute command */
/* TODO: If execvp returns, it failed */
} else {
/* TODO: Parent: wait for child */
}
}
return 0;
}
