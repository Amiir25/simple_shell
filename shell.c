#include "simple_shell.h"

/**
 * read_command - Reads a command from stdin
 *
 * Return: The command string, or NULL on failure
 */

char *read_command(void)
{
	char *line = NULL;
	size_t len = 0;

	if (getline(&line, &len, stdin) == -1)
	{
		free(line);
		return (NULL);
	}

	handle_comments(line);
	return (line);
}

/**
 * parse_command - Splits a command into an array of arguments
 * @line: The command string
 *
 * Return: An array of arguments, or NULL on failure
 */

char **parse_command(char *line)
{
	int bufsize, position;
	char **tokens;
	char *token;

	bufsize = MAX_ARGS;
	position = 0;
	tokens = malloc(bufsize * sizeof(char *));
	if (!tokens)
	{
		fprintf(stderr, "hsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, DELIM);
	while (token != NULL)
	{
		tokens[position++] = token;

		if (position >= bufsize)
		{
			bufsize += MAX_ARGS;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "hsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, DELIM);
	}

	tokens[position] = NULL;
	return (tokens);
}

/**
 * execute_command - Executes a command
 * @args: The array of arguments
 *
 * Return: 1 if the shell should continue, 0 if it should terminate
 */

int execute_command(char **args)
{
	int i;
	char *builtin_str[50];

	int (*builtin_func[100])(char **);
	int num_builtins;
	char *command;

	builtin_str[] = {"exit", "env", "cd", "setenv", "unsetenv", "alias"};
	(*builtin_func[100])(char **) = {&shell_exit, &shell_env, &shell_cd,
					&shell_setenv, &shell_unsetenv, &shell_alias};
	num_builtins = sizeof(builtin_str) / sizeof(char *);
	if (args[0] == NULL)
		return (1);
	for (i = 0; i < num_builtins; i++)
	{
		if (_strcmp(args[0], builtin_str[i]) == 0)
		return ((*builtin_func[i])(args));
	}
	command = search_path(args[0]);
	if (command)
	{
		pid_t pid = fork();

		if (pid == 0)
		{
			if (execve(command, args, environ) == -1)
			perror(args[0]);
			exit(EXIT_FAILURE);
		}
		else if (pid < 0)
			perror("hsh");
		else
		{
			int status;

			do {
				waitpid(pid, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
		free(command);
	}
	else
		fprintf(stderr, "%s: command not found\n", args[0]);
	return (1);
}

/**
 * execute_file - Executes commands from a file
 * @filename: The name of the file containing commands
 */

void execute_file(const char *filename)
{
	FILE *file;
	char *line;
	char **args;
	size_t len;
	ssize_t nread;

	file = fopen(filename, "r");
	line = NULL;
	len = 0;

	if (file == NULL)
	{
		perror("hsh");
		exit(EXIT_FAILURE);
	}

	while ((nread = getline(&line, &len, file)) != -1)
	{
		handle_comments(line);
		args = parse_command(line);
		execute_command(args);
		free(args);
	}

	free(line);
	fclose(file);
}
