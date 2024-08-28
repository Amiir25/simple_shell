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
    {
        execute_from_file(argv[1], argv[0]);
    }
    else if (argc == 1)
    {
        shell_loop(argv[0]);
    }
    else
    {
        fprintf(stderr, "%s: Usage: %s [filename]\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    return (0);
}

