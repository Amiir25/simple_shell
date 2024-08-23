#include "simple_shell.h"

int shell_exit(char **args)
{
    int status = 0;

    if (args[1])
        status = atoi(args[1]);

    exit(status);
}

int shell_env(char **args)
{
    char **env = environ;

    (void)args;  // Suppress unused variable warning

    while (*env) {
        printf("%s\n", *env);
        env++;
    }

    return 1;
}

int shell_cd(char **args)
{
    char *dir = args[1];
    char cwd[1024];

    if (dir == NULL) {
        dir = getenv("HOME");
    } else if (_strcmp(dir, "-") == 0) {
        dir = getenv("OLDPWD");
        if (dir)
            printf("%s\n", dir);
    }

    if (chdir(dir) != 0) {
        perror("hsh");
        return 1;
    }

    setenv("OLDPWD", getenv("PWD"), 1);
    setenv("PWD", getcwd(cwd, sizeof(cwd)), 1);

    return 1;
}

int shell_setenv(char **args)
{
    if (args[1] && args[2]) {
        if (setenv(args[1], args[2], 1) != 0) {
            perror("hsh");
        }
    } else {
        fprintf(stderr, "hsh: setenv VARIABLE VALUE\n");
    }
    return 1;
}

int shell_unsetenv(char **args)
{
    if (args[1]) {
        if (unsetenv(args[1]) != 0) {
            perror("hsh");
        }
    } else {
        fprintf(stderr, "hsh: unsetenv VARIABLE\n");
    }
    return 1;
}

int shell_alias(char **args)
{
    // Placeholder for alias functionality
    (void)args;
    return 1;
}
