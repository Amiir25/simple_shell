#include "simple_shell.h"

/**
 * custom_getline - Reads a line from stdin.
 *
 * Return: The read line, or NULL on failure.
 */

char *custom_getline(void)
{
	char *line;
	size_t len0;
	ssize_t read;

	line = NULL;
	len0 = 0;

	read = getline(&line, &len0, stdin);
	if (read == -1)
	{
		free(line);
		return (NULL);
	}

	return (line);
}

/**
 * custom_tokenize - Tokenizes a line into arguments.
 * @line: The line to tokenize.
 * @args: The array to store the tokens.
 */

void custom_tokenize(char *line, char **args)
{
	int i;
	char *token;

	i = 0;
	token = strtok(line, DELIM);
	while (token != NULL && i < MAX_ARGS - 1)
	{
		args[i++] = token;
		token = strtok(NULL, DELIM);
	}
	args[i] = NULL;
}

/**
 * strip_comments - Removes comments from the command line.
 * @line: The input line to process.
 */

void strip_comments(char *line)
{
	char *comment;

	comment = strchr(line, '#');
	if (comment)
		*comment = '\0';
}

/**
 * split_commands - Splits a line into multiple commands separated by ';'.
 * @line: The input line to split.
 * @commands: The array to store the commands.
 */

void split_commands(char *line, char **commands)
{
	int i;
	char *command;

	i = 0;
	command = strtok(line, ";");
	while (command != NULL)
	{
		commands[i++] = command;
		command = strtok(NULL, ";");
	}
	commands[i] = NULL;
}
