#include "simple_shell.h"

/**
 * main - Entry point for the shell.
 * @argc: Argument count.
 * @argv: Argument vector.
 *
 * Return: Exit status of the shell.
 */

int main(int argc, char **argv)
{
	if (argc == 2)
		execute_from_file(argv[1], argv[0]);

	else if (argc == 1)
		shell_loop(argv[0]);

	else
	{
		fprintf(stderr, "%s: Usage: %s [filename]\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
	}

	return (0);
}

/**
 * shell_loop - Main loop for the shell.
 * @program_name: The name of the program (argv[0]).
 */

void shell_loop(char *program_name)
{
	int i;
	char *line;
	char *commands[MAX_COMMANDS];
	int status;

	status = 1;
	while (status)
	{
		printf("$ ");
		line = custom_getline();
		if (line == NULL)
		{
			if (feof(stdin))
			{
				printf("\n");
				break;
			}
			perror(program_name);
			continue;
		}

		strip_comments(line);
		split_commands(line, commands);

		for (i = 0; commands[i] != NULL && status; i++)
		status = execute_command_line(commands[i], program_name);

		free(line);
	}
}

