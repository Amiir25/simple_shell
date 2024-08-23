#include "simple_shell.h"

/**
 * shell_env - Prints the environment variables
 * @args: Arguments passed to the command (not used)
 *
 * Return: 1 on success
 */

int shell_env(char **args)
{
	char **env;

	env = environ;

	(void)args;  /* Suppress unused variable warning */

	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}

	return (1);
}

/**
 * shell_cd - Changes the current directory
 * @args: Arguments passed to the command
 *
 * Return: 1 on success, -1 on failure
 */

int shell_cd(char **args)
{
	char *dir = args[1];
	char cwd[1024];

	if (dir == NULL)
		dir = getenv("HOME");

	else if (_strcmp(dir, "-") == 0)
	{
		dir = getenv("OLDPWD");
		if (dir)
			printf("%s\n", dir);
	}

	if (chdir(dir) != 0)
	{
		perror("hsh");
		return (1);
	}

	setenv("OLDPWD", getenv("PWD"), 1);
	setenv("PWD", getcwd(cwd, sizeof(cwd)), 1);

	return (1);
}

/**
 * shell_setenv - Initializes or modifies an environment variable
 * @args: Arguments passed to the command
 *
 * Return: 1 on success, -1 on failure
 */

int shell_setenv(char **args)
{
	if (args[1] && args[2])
	{
		if (setenv(args[1], args[2], 1) != 0)
			perror("hsh");
	}

	else
		fprintf(stderr, "hsh: setenv VARIABLE VALUE\n");

	return (1);
}

/**
 * shell_unsetenv - Removes an environment variable
 * @args: Arguments passed to the command
 *
 * Return: 1 on success, -1 on failure
 */

int shell_unsetenv(char **args)
{
	if (args[1])
	{
		if (unsetenv(args[1]) != 0)
			perror("hsh");
	}

	else
		fprintf(stderr, "hsh: unsetenv VARIABLE\n");

	return (1);
}

/**
 * shell_alias - Handles the alias command
 * @args: Arguments passed to the command
 *
 * Return: 1 on success
 */

int shell_alias(char **args)
{
	/* Placeholder for alias functionality */
	(void)args;
	return (1);
}
