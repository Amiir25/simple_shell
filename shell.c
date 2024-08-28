#include "simple_shell.h"

int last_exit_status = 0;

/**
 * execute_command_line - Executes a single command line.
 * @line: The command line to execute.
 * @program_name: The name of the program (argv[0]).
 *
 * Return: 1 if the shell should continue running, 0 if it should terminate.
 */

int execute_command_line(char *line, char *program_name)
{
	int status;
	char *args[MAX_ARGS];

	status = 1;
	custom_tokenize(line, args);

	if (args[0] == NULL)
		return (1);

	if (strchr(line, '&') || strchr(line, '|'))
		return (handle_logical_operators(line, program_name));

	replace_variables(args);

	status = execute_command(args, program_name);
	return (status);
}

/**
 * handle_logical_operators - Handles '&&' and '||' logical operators.
 * @line: The input line containing the command.
 * @program_name: The name of the program (argv[0]).
 *
 * Return: 1 if the shell should continue running, 0 if it should terminate.
 */

int handle_logical_operators(char *line, char *program_name)
{
	char *left, *right;
	int status;

	left = strtok(line, "&&");
	if (left != NULL)
	{
		right = strtok(NULL, "&&");
		if (right != NULL)
		{
			status = execute_command_line(left, program_name);
			if (status == 1)
				return (execute_command_line(right, program_name));

			return (status);
		}
	}

	left = strtok(line, "||");
	if (left != NULL)
	{
		right = strtok(NULL, "||");
		if (right != NULL)
		{
			status = execute_command_line(left, program_name);
			if (status != 1)
				return (execute_command_line(right, program_name));
		}
		return (status);
	}

	return (execute_command_line(line, program_name));
}

/**
 * execute_command - Executes a command.
 * @args: Array of arguments.
 * @program_name: The name of the program (argv[0]).
 *
 * Return: 1 if the shell should continue running, 0 if it should terminate.
 */

int execute_command(char **args, char *program_name)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (args[0] == NULL)
		return (1);

	if (handle_builtin(args, program_name))
		return (1);

	cmd_path = find_command(args[0]);
	if (cmd_path == NULL)
	{
		fprintf(stderr, "%s: command not found: %s\n", program_name, args[0]);
		return (1);
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, args, environ) == -1)
			perror(program_name);

		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("fork");

	else
	{
		wait(&status);
		last_exit_status = status;
	}

	free(cmd_path);
	return (1);
}

/**
 * handle_builtin - Handles built-in commands.
 * @args: Array of arguments.
 * @program_name: The name of the program (argv[0]).
 *
 * Return: 1 if the command was handled as a built-in, 0 otherwise.
 */

int handle_builtin(char **args, char *program_name)
{
	if (strcmp(args[0], "exit") == 0)
	{
		if (args[1])
			exit(atoi(args[1]));
		return (1);
	}
	if (strcmp(args[0], "env") == 0)
	{
		print_env();
		return (1);
	}
	if (strcmp(args[0], "alias") == 0)
	{
		handle_alias(args);
		return (1);
	}
	if (strcmp(args[0], "setenv") == 0)
	{
		if (args[1] && args[2])
			set_env(args[1], args[2]);
		else
			fprintf(stderr, "%s: setenv: Missing arguments\n", program_name);
		return (1);
	}
	if (strcmp(args[0], "unsetenv") == 0)
	{
		if (args[1])
			unset_env(args[1]);
		else
			fprintf(stderr, "%s: unsetenv: Missing argument\n", program_name);
		return (1);
	}
	if (strcmp(args[0], "cd") == 0)
	{
		change_directory(args[1]);
		return (1);
	}
	return (0);
}

/**
 * replace_variables - Handles variable replacements in the arguments.
 * @args: The array of arguments.
 */

void replace_variables(char **args)
{
	int i;

	for (i = 0; args[i] != NULL; i++)
	{
		if (strcmp(args[i], "$$") == 0)
		{
			char pid_str[12];

			snprintf(pid_str, 12, "%d", getpid());
			free(args[i]);
			args[i] = strdup(pid_str);
		}
		else if (strcmp(args[i], "$?") == 0)
		{
			char status_str[12];

			snprintf(status_str, 12, "%d", WEXITSTATUS(last_exit_status));
			free(args[i]);
			args[i] = strdup(status_str);
		}
		else if (args[i][0] == '$' && args[i][1] != '\0')
		{
			char *env_value = getenv(args[i] + 1);

			if (env_value)
			{
				free(args[i]);
				args[i] = strdup(env_value);
			}
		}
	}
}
