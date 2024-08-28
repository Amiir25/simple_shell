#include "simple_shell.h"

/**
 * main - Entry point for the shell program.
 * @argc: Argument count.
 * @argv: Argument vector.
 * 
 * Return: Always 0 on success.
 */
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    shell_loop(argv[0]);
    return (0);
}
