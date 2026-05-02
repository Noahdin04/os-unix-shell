
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

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


                /* -=-=-=-=-=-=-= background detection -=-=-=-=-=-=-= */

                /* check for trailing & before parsing */
                int is_background = 0;
                int i = strlen(input);
                /* starts at the end of the input and searches backwards for & */
                while (i > 0 && (input[i-1] == ' ' || input[i-1] == '\t')) {
                        i--;
                }
                if (i > 0 && input[i-1] == '&') {
                        is_background = 1;
                        input[i-1] = '\0';
                        i--;
                        /* trim spaces between last arg and '&' */
                        while (i > 0 && (input[i-1] == ' ' || input[i-1] == '\t')) {
                                input[--i] = '\0';
                        }
                }

                pipe_pos = strchr(input, '|');

                if (pipe_pos != NULL) {

                    *pipe_pos = '\0';
                    pipe_pos++;

                    while (*pipe_pos == ' ' || *pipe_pos == '\t') {
                        pipe_pos++;
                    }

                    if (parse_command(input, left_argv, MAX_ARGS) == 0) {
                        continue;
                    }

                    if (parse_command(pipe_pos, right_argv, MAX_ARGS) == 0) {
                        continue;
                    }

                    int pfd[2];

                    if (pipe(pfd) < 0) {
                        perror("pipe");
                        continue;
                    }

                    pid_t left_pid = fork();

                    if (left_pid < 0) {
                        perror("fork");
                        continue;
                    }

                    if (left_pid == 0) {
                        dup2(pfd[1], STDOUT_FILENO);

                        close(pfd[0]);
                        close(pfd[1]);

                        execvp(left_argv[0], left_argv);
                        perror("execvp");
                        exit(1);
                    }

                    pid_t right_pid = fork();

                    if (right_pid < 0) {
                        perror("fork");
                        continue;
                    }

                    if (right_pid == 0) {
                        dup2(pfd[0], STDIN_FILENO);

                        close(pfd[1]);
                        close(pfd[0]);

                        execvp(right_argv[0], right_argv);
                        perror("execvp");
                        exit(1);
                    }

                    close(pfd[0]);
                    close(pfd[1]);

                    if (is_background) {
                        printf("[running in background]\n");
                    } else {
                        waitpid(left_pid, NULL, 0);
                        waitpid(right_pid, NULL, 0);
                    }

                    continue;
                }


                /* -=-=-=-=-=-=-= parse command -=-=-=-=-=-=-= */

                /* TODO: parse_command(input, argv, MAX_ARGS); if no args, continue */
                if (parse_command(input, argv, MAX_ARGS) == 0) {
                        continue;
                }

                /* -=-=-=-=-=-=-= Redirection Parsing -=-=-=-=-=-=-= */

                char *in_file = NULL;
                char *out_file = NULL;
        
                char *clean_argv[MAX_ARGS];
                int j = 0;
        
                for (int i = 0; argv[i] != NULL; i++) {
        
                    if (strcmp(argv[i], "<") == 0) {
                        if (argv[i + 1] == NULL) {
                            printf("Error,  requires a filename.\n");
                            continue;
                        }
                        in_file = argv[i + 1];
                        i++; 
                    }
                    else if (strcmp(argv[i], ">") == 0) {
                        if (argv[i + 1] == NULL) {
                            printf("Error, requires a filename.\n");
                            continue;
                        }
                        out_file = argv[i + 1];
                        i++; 
                    }
                    else {
                        clean_argv[j++] = argv[i];
                    }
                }
        
                clean_argv[j] = NULL;
        
                if (clean_argv[0] == NULL) {
                    continue;
                }

                /* -=-=-=-=-=-=-= End of Redirection Parsing -=-=-=-=-=-=-= */

                
                pid_t pid = fork();

                if (pid < 0) {
                        perror("fork");
                        continue;
                }
                if (pid == 0) {
                        /* TODO: Child: execute command */

                        /* -=-=-=-=-=-=-= Input Redirection -=-=-=-=-=-=-= */
                        
                        if (in_file != NULL) {
                                int fd = open(in_file, O_RDONLY);
                                if (fd < 0) {
                                    perror("error opening input file");
                                    exit(1);
                                }
                                dup2(fd, STDIN_FILENO);
                                close(fd);
                            }
                
                            /* -=-=-=-=-=-=-= Output Redirection -=-=-=-=-=-=-= */

                            if (out_file != NULL) {
                                int fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                                if (fd < 0) {
                                    perror("error opening output file");
                                    exit(1);
                                }
                                dup2(fd, STDOUT_FILENO);
                                close(fd);
                            }
                
                            execvp(clean_argv[0], clean_argv);
                            perror("execvp");
                            exit(1);
                /* TODO: If execvp returns, it failed */
                } else {
                        if (is_background) {
                                printf("[running in background]\n");
                        } else {
                                int status;
                                if (waitpid(pid, &status, 0) < 0) {
                                        perror("waitpid");
                                }
                        }
                }
        }
        return 0;
}
