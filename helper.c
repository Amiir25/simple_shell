#include "shell.h"

/**
 * _strcmp - Compares two strings
 * @s1: The first string
 * @s2: The second string
 *
 * Return: 0 if strings are equal, a negative or positive value if they differ
 */
int _strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

/**
 * _strlen - Returns the length of a string
 * @str: The string to measure
 *
 * Return: Length of the string
 */
int _strlen(const char *str)
{
    int len = 0;

    while (str[len])
        len++;
    return (len);
}

/**
 * _strdup - Duplicates a string
 * @str: The string to duplicate
 *
 * Return: Pointer to the duplicated string
 */
char *_strdup(const char *str)
{
    char *dup;
    int len = _strlen(str);

    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    strcpy(dup, str);
    return (dup);
}

/**
 * handle_comments - Removes comments from the input line
 * @line: The input line
 */
void handle_comments(char *line)
{
    char *comment_start = strchr(line, '#');

    if (comment_start)
        *comment_start = '\0';
}

/**
 * search_path - Searches for a command in the system's PATH
 * @command: The command to search for
 *
 * Return: Path to the command, or NULL if not found
 */
char *search_path(char *command)
{
    char *path = getenv("PATH");
    char *token;
    char full_path[1024];

    if (command[0] == '/' || command[0] == '.') {
        return (_strdup(command));
    }

    token = strtok(path, ":");
    while (token != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
        if (access(full_path, X_OK) == 0)
            return (_strdup(full_path));
        token = strtok(NULL, ":");
    }

    return (NULL);
}
