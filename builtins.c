#include "simple_shell.h"

/**
 * print_env - Prints the current environment variables.
 */
void print_env(void)
{
    for (int i = 0; environ[i] != NULL; i++)
    {
        printf("%s\n", environ[i]);
    }
}

/**
 * set_env - Initializes or modifies an environment variable.
 * @name: The name of the variable.
 * @value: The value of the variable.
 */
void set_env(const char *name, const char *value)
{
    if (setenv(name, value, 1) != 0)
    {
        perror("setenv");
    }
}

/**
 * unset_env - Removes an environment variable.
 * @name: The name of the variable to remove.
 */
void unset_env(const char *name)
{
    if (unsetenv(name) != 0)
    {
        perror("unsetenv");
    }
}

/**
 * change_directory - Changes the current directory.
 * @path: The path to change to.
 */
void change_directory(const char *path)
{
    if (path == NULL || strcmp(path, "~") == 0)
    {
        path = getenv("HOME");
    }
    else if (strcmp(path, "-") == 0)
    {
        path = getenv("OLDPWD");
    }

    if (chdir(path) != 0)
    {
        perror("cd");
    }
    else
    {
        set_env("OLDPWD", getenv("PWD"));
        set_env("PWD", getcwd(NULL, 0));
    }
}

/**
 * handle_alias - Handles alias commands.
 * @args: The array of arguments.
 */
void handle_alias(char **args)
{
    /* This is a placeholder implementation.
     * Implement alias management as needed.
     */
    if (args[1] == NULL)
    {
        /* Print all aliases */
        printf("Alias feature is not fully implemented yet.\n");
    }
    else
    {
        /* Set or print specific alias */
        printf("Alias feature is not fully implemented yet.\n");
    }
}
