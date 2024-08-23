#include "simple_shell.h"

char *read_command(void)
{
    char *line = NULL;
    size_t len = 0;

    if (getline(&line, &len, stdin) == -1) {
        free(line);
        return NULL;
    }

    handle_comments(line);
    return line;
}

char **parse_command(char *line)
{
    int bufsize = MAX_ARGS, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "hsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, DELIM);
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "hsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int execute_command(char **args)
{
    char *builtin_str[] = {"exit", "env", "cd", "setenv", "unsetenv", "alias"};
    int (*builtin_func[])(char **) = {&shell_exit, &shell_env, &shell_cd, &shell_setenv, &shell_unsetenv, &shell_alias};
    int num_builtins = sizeof(builtin_str) / sizeof(char *);
    char *command;

    if (args[0] == NULL)
        return 1;

    for (int i = 0; i < num_builtins; i++) {
        if (_strcmp(args[0], builtin_str[i]) == 0)
            return (*builtin_func[i])(args);
    }

    command = search_path(args[0]);

    if (command) {
        pid_t pid = fork();

        if (pid == 0) {
            if (execve(command, args, environ) == -1)
                perror(args[0]);
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("hsh");
        } else {
            int status;
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

        free(command);
    } else {
        fprintf(stderr, "%s: command not found\n", args[0]);
    }

    return 1;
}

void execute_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    char *line = NULL;
    char **args;
    size_t len = 0;
    ssize_t nread;

    if (file == NULL) {
        perror("hsh");
        exit(EXIT_FAILURE);
    }

    while ((nread = getline(&line, &len, file)) != -1) {
        handle_comments(line);
        args = parse_command(line);
        execute_command(args);
        free(args);
    }

    free(line);
    fclose(file);
}
