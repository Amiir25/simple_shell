#include "simple_shell.h"

/**
 * getline - Custom implementation of the getline function
 * @lineptr: Pointer to the buffer where the line is stored
 * @n: Size of the buffer
 * @stream: Stream to read from (usually stdin)
 *
 * Return: Number of characters read, or -1 on failure
 */

ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
	static char buffer[1024];
	static size_t pos, length;
	ssize_t num_read;
	char *newline;

	num_read = 0;
	if (*lineptr == NULL || *n == 0)
	{
		*n = 1024;
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return (-1);
	}

	while (1)
	{
		if (pos >= length)
		{
			pos = 0;
			length = read(fileno(stream), buffer, sizeof(buffer));
			if (length <= 0)
				return (-1);
		}
		(*lineptr)[num_read++] = buffer[pos++];

		if (num_read >= *n)
		{
			*n += 1024;
			*lineptr = realloc(*lineptr, *n);
			if (*lineptr == NULL)
				return (-1);
		}
		newline = strchr(*lineptr, '\n');
		if (newline != NULL)
		{
			*newline = '\0';
			break;
		}
	}
	return (num_read);
}
