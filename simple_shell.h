#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_COMMANDS 10
#define MAX_ARGS 20
#define DELIM " \t\r\n\a"

extern char **environ;
extern int last_exit_status;

/* Function prototypes */
void shell_loop(char *program_name);
int execute_command_line(char *line, char *program_name);
int execute_command(char **args, char *program_name);
int handle_builtin(char **args, char *program_name);
void replace_variables(char **args);
int handle_logical_operators(char *line, char *program_name);

void print_env(void);
void set_env(const char *name, const char *value);
void unset_env(const char *name);
void change_directory(const char *path);
void handle_alias(char **args);

void strip_comments(char *line);
void split_commands(char *line, char **commands);
void custom_tokenize(char *line, char **args);
char *custom_getline(void);
char *find_command(char *command);

void execute_from_file(const char *filename, char *program_name);

#endif /* SHELL_H */
