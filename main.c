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

	do {
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

/**
 * launch_process - Forks a child process to execute a command
 * @args: Array of arguments (command and its arguments)
 *
 * Return: 1 if the shell should continue running, 0 if it should terminate
 */

int launch_process(char **args)
{
	pid_t pid = fork();

	if (pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
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

	return (1);
}

/**
 * shell_exit - Exits the shell
 * @args: Arguments passed to the command
 *
 * Return: Doesn't return (terminates the shell)
 */

int shell_exit(char **args)
{
	int status;

	status = 0;

	if (args[1])
		status = atoi(args[1]);

	exit(status);
}
