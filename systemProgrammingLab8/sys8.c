#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#define BUFSIZE (30000)
void runShell(char** args1, int argsSize) {
    int mode = 0;
    int fd;
    int fd_pipe[2];
    char **args2;
    char path[1000] = "/bin/";
    pipe(fd_pipe);
    for (int i = 0; args1[i] != NULL; i++)
    {
        if(!strcmp(args1[i], "<")) {
            mode = 1;
            args2 = &args1[i] + 1;
            fd = open(args2 + 1, O_RDONLY);
            args1[i] = NULL;
            break;
        }
        else if (!strcmp(args1[i], ">")) {
            mode = 2;
            args2 = &args1[i] + 1;
            fd = open(args2 + 1, O_WRONLY | O_CREAT | O_TRUNC, 755);
            args1[i] = NULL;
            break;
        }
        else if (!strcmp(args1[i], "|")) {
            mode = 3;
            args2 = &args1[i] + 1;
            fd = open(args2 + 1, O_RDWR | O_CREAT | O_TRUNC, 755);
            break;
        }
    }
    strcat(path, args1[0]);

    
    if (fork() == 0)
    {
        switch (mode) {
        case 1:
            close(fd_pipe[1]);
            dup2(fd, fd_pipe[0]);
            break;
        case 2:
            close(fd_pipe[0]);
            dup2(fd, fd_pipe[1]);
            break;
        }
        execv(path, args1);
        close(fd);
        _exit(0);
    }
    else {
        switch (mode) {
        case 1:
            close(fd_pipe[0]);
            break;
        case 2:
            close(fd_pipe[1]);
            break;
        }
        wait(NULL);
    }
}
int main() {
    char buffer[BUFSIZE];
    char* args[BUFSIZE / 2];
    int argsSize;
    while (1) {
        read(STDIN_FILENO, buffer, sizeof(buffer));
        *(strchr(buffer, '\n')) = '\0';
        args[(argsSize = 0)] = strtok(buffer, " ");
        if (args[0] == NULL) continue;
        while ((args[++argsSize] = strtok(NULL, " ")) != NULL);

        if (!strcmp(args[0], "quit")) break;
        else {
            if (fork() == 0) runShell(args, argsSize);
            else wait(NULL);
        }
    }
}