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
	int argCount = 0;

	char *current = strtok(line, " \t");

	while (current != NULL && argCount < max_args - 1) {
		argv[argCount++] = current;
		current = strtok(NULL, " \t");
	}

	argv[argCount] = NULL;

	return argCount;
}

int main(void)
	{
	char input[MAX_INPUT];
	char *argv[MAX_ARGS];
	while (1) {

		/* print prompt and flush */
		printf("myshell> ");
		fflush(stdout);

		/* read a line with fgets; handle EOF */
		if (fgets(input, MAX_INPUT, stdin) == NULL) {
			/* Optional: treat Ctrl+D as exit */
			break;
		}

		/* strip newline */
		input[strcspn(input, "\n")] = 0;

		/* removes leading whitespace characters from the buffer */
		char *p = input;
		while (*p == ' ' || *p == '\t') {
			p++;
		}
		strcpy(input, p);

		/* if empty line, continue */
		if (input[0] == '\0') {
			continue;
		}

		/* if input == "exit", print message and break */
		if (strcmp(input, "exit") == 0) {
			printf("Shell terminated.\n");
			break;
		}

		/* TODO: parse_command(input, argv, MAX_ARGS); if no args, continue */
		if (parse_command(input, argv, MAX_ARGS) == 0) {
			continue;
		}

		pid_t pid = fork();
		if (pid < 0) {
			perror("fork");
			continue;
		}
		if (pid == 0) {
			/* TODO: Child: execute command */
			execvp(argv[0], argv);
			perror("execvp");
			exit(1);
		/* TODO: If execvp returns, it failed */
		} else {
			/* TODO: Parent: wait for child */
		}
	}
	return 0;
}
