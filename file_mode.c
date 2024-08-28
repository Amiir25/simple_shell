#include "simple_shell.h"

/**
 * execute_from_file - Executes commands from a file.
 * @filename: The name of the file containing the commands.
 * @program_name: The name of the program (argv[0]).
 */
void execute_from_file(const char *filename, char *program_name)
{
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    char *commands[MAX_COMMANDS];
    int status = 1;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        perror(program_name);
        exit(EXIT_FAILURE);
    }

    while (getline(&line, &len, file) != -1)
    {
        strip_comments(line);
        split_commands(line, commands);

        for (int i = 0; commands[i] != NULL && status; i++)
        {
            status = execute_command_line(commands[i], program_name);
        }
    }

    free(line);
    fclose(file);

    /* Exit with the status of the last command executed */
    exit(WEXITSTATUS(last_exit_status));
}
