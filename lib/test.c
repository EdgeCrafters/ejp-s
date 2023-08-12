#include "test.h"

const int stdioFd[] = {STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO};

int execute(char path[], char input[], char output[])
{

    int newFd[3][2];
    for (int i = 0; i < 3; ++i)
    {
        if (pipe(newFd[i]) < 0)
            exit(EXIT_FAILURE);
    }

    int status;
    pid_t pid, childPid;

    childPid = fork();
    if (childPid == -1)
    {
        fprintf(stderr, "fail to fork...\n");
        exit(EXIT_FAILURE);
    }
    else if (childPid == 0)
    {
        write(newFd[0][PIPE_IN], input, strlen(input) + 1);

        for (int i = 0; i < 3; ++i)
        {
            int dir = (i == 0 ? PIPE_OUT : PIPE_IN);
            if (dup2(newFd[i][dir], stdioFd[i]) < 0)
                exit(EXIT_FAILURE);
            if (close(newFd[i][dir]) < 0)
                exit(EXIT_FAILURE);
            if (close(newFd[i][1 - dir]) < 0)
                exit(EXIT_FAILURE);
        }

        char *argvPtr[] = {path, NULL};
        execv(path, argvPtr);
        abort();
    }

    for (int i = 0; i < 3; ++i)
    {
        if (i == 0)
        {
            close(newFd[i][PIPE_IN]);
            close(newFd[i][PIPE_OUT]);
        }
        else
        {
            close(newFd[i][PIPE_IN]);
        }
    }

    pid = wait(&status);
    if (pid != childPid)
        exit(EXIT_FAILURE);

    read(newFd[1][PIPE_OUT], output, 512);
    close(newFd[1][PIPE_OUT]);

    return 0;
}