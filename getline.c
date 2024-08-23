#include "simple_shell.h"

ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
    static char buffer[1024];
    static size_t pos, length;
    ssize_t bytes_read = 0;
    char *line = *lineptr;

    if (!line || !*n) {
        *n = 1024;
        line = malloc(*n);
        if (!line)
            return -1;
    }

    while (1) {
        if (pos >= length) {
            pos = 0;
            length = read(fileno(stream), buffer, sizeof(buffer));
            if (length <= 0)
                break;
        }

        line[bytes_read++] = buffer[pos++];
        if (bytes_read >= (ssize_t)(*n - 1)) {
            *n += 1024;
            line = realloc(line, *n);
            if (!line)
                return -1;
        }

        if (line[bytes_read - 1] == '\n')
            break;
    }

    line[bytes_read] = '\0';
    *lineptr = line;

    if (length == 0 && bytes_read == 0)
        return -1;

    return bytes_read;
}
