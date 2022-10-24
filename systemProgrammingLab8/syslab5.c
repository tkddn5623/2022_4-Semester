#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
void runShell(char* const* args) {
    char path[1000] = "/bin/";
    strcat(path, args[0]);
    if (fork() == 0) {
        execv(path, args);
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