#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
void runShell(char** args1) {
    int mode = 0;
    char path[1000] = "/bin/";
    int fd = -1;
    int pipes[2] = { -1, -1 };
    char** args2;
    strcat(path, args1[0]);
    for (int i = 0; args1[i] != NULL; i++) {
        const char ch = args1[i][0];
        if (ch == '<' || ch == '>' || ch == '|') {
            args2 = &args1[i] + 1;
            args1[i] = NULL;
            switch (ch) {
            case '<': mode = 1; break;
            case '>': mode = 2; break;
            case '|': mode = 3; break;
            }
            break;
        }
    }
    if (fork() == 0) {
        switch (mode) {
        case 0:
            execv(path, args1);
            break;
        case 1:
            if ((fd = open(args2[0], O_RDONLY)) < 0) _exit(1);
            dup2(fd, STDIN_FILENO);
            execv(path, args1);
            break;
        case 2:
            if ((fd = open(args2[0], O_WRONLY | O_CREAT | O_TRUNC, 755)) < 0) _exit(1);
            dup2(fd, STDOUT_FILENO);
            execv(path, args1);
            break;
        case 3:
            pipe(pipes);
            if (fork() == 0) {
                close(pipes[1]);
                dup2(pipes[0], STDIN_FILENO);
                strcpy(path, "/bin/");
                strcat(path, args2[0]);
                execv(path, args2);
                _exit(0);
            }
            close(pipes[0]);
            dup2(pipes[1], STDOUT_FILENO);
            execv(path, args1);
            wait(NULL);
            break;
        }
        _exit(0);
    }
    else {
        wait(NULL);
    }
}
int main() {
    char buffer[30000];
    char* args[15000];
    int argsSize;
    while (1) {
        read(STDIN_FILENO, buffer, sizeof(buffer));
        *(strchr(buffer, '\n')) = '\0';
        args[(argsSize = 0)] = strtok(buffer, " ");
        if (args[0] == NULL) continue;
        while ((args[++argsSize] = strtok(NULL, " ")) != NULL);

        if (!strcmp(args[0], "quit")) break;
        else runShell(args);
    }
}