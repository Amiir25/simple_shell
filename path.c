#include "simple_shell.h"

/**
 * find_command - Finds the full path of a command in the PATH.
 * @command: The command to find.
 *
 * Return: The full path of the command, or NULL if not found.
 */

char *find_command(char *command)
{
	char *path;
	char *token;
	char full_path[1024];

	path = getenv("PATH");
	if (command[0] == '/' || command[0] == '.')
		return (strdup(command));

	token = strtok(path, ":");
	while (token != NULL)
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
		if (access(full_path, X_OK) == 0)
			return (strdup(full_path));

		token = strtok(NULL, ":");
	}

	return (NULL);
}
