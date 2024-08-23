#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

extern char **environ;

#define MAX_ARGS 64
#define DELIM " \t\r\n\a"

/* Function Prototypes */
char *read_command(void);
char **parse_command(char *line);
int execute_command(char **args);
void execute_file(const char *filename);

/* Built-in commands */
int shell_exit(char **args);
int shell_env(char **args);
int shell_cd(char **args);
int shell_setenv(char **args);
int shell_unsetenv(char **args);
int shell_alias(char **args);

/* Helper functions */
int _strcmp(const char *s1, const char *s2);
int _strlen(const char *str);
char *_strdup(const char *str);
void handle_comments(char *line);
char *search_path(char *command);

#endif /* SHELL_H */
