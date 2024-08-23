#include "simple_shell.h"

/**
 * main - Entry point of the shell program
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 on success, or other exit code on failure
 */
 
int main(int argc, char **argv)
{
	char *command;
	char **args;
	int status;

	command = NULL;
	args = NULL;
	status = 1;

	if (argc == 2)
	{
		/* Execute commands from a file */
		execute_file(argv[1]);
		exit(EXIT_SUCCESS);
	}

	do
	{
		if (isatty(STDIN_FILENO))
			printf("($) ");  /* Prompt */
			command = read_command();
		if (command == NULL)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break;
		}
	args = parse_command(command);
	status = execute_command(args);

	free(command);
	free(args);

	} while (status);

	return (0);
}
