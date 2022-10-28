#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>
#define MAX_BUF (50000)
#define MAX_PATH (5000)
#define MAX_CMD (1000)
#include <stdio.h> //<< Please delete.

int ls(char* dir_path, char* option);
int head(char* file_path, char* line);
int tail(char* file_path, char* line);
int mv(char* file_path1, char* file_path2);
int mk_dir(char* dir_path);
int rm_dir(char* dir_path);

int main() {
    int i, cmdret = 0;
    int argSize, cmdSize;
    char buffer[MAX_BUF];
    char* commands[MAX_CMD];
    char* arguments[MAX_CMD];
    const char* const _err_invalid = "ERROR: The command is executed abnormally";
    size_t size;
    while (1) {
        /* Input commands */
        memset(buffer, 0, sizeof(buffer));
        read(STDIN_FILENO, buffer, sizeof(buffer));
        commands[(cmdSize = 0)] = strtok(buffer, "\n");
        if (commands[0] != NULL) {
            while ((commands[++cmdSize] = strtok(NULL, "\n")) != NULL);
        }
        else commands[0] = buffer;
        /* ============== */
        for (int i = 0; i < cmdSize; i++) {
            /* Tokenize commands (Use strtok function) */
            memset(arguments, 0, sizeof(arguments));
            arguments[(argSize = 0)] = strtok(commands[i], " ");
            while ((arguments[++argSize] = strtok(NULL, " ")) != NULL);
            /* ======================================= */
            
            /* Command */
            if (strcmp("ls", arguments[0]) == 0) {
                cmdret = ls(arguments[1], arguments[2]);
            }
            else if (strcmp("head", arguments[0]) == 0) {
                cmdret = head(arguments[3], arguments[2]);
            }
            else if (strcmp("tail", arguments[0]) == 0) {
                cmdret = tail(arguments[3], arguments[2]);
            }
            else if (strcmp("mv", arguments[0]) == 0) {
                cmdret = mv(arguments[1], arguments[2]);
            }
            else if (strcmp("mkdir", arguments[0]) == 0) {
                cmdret = mk_dir(arguments[1]);
            }
            else if (strcmp("rmdir", arguments[0]) == 0) {
                cmdret = rm_dir(arguments[1]);
            }
            else if (strcmp("quit", arguments[0]) == 0) {
                goto _BREAK;
            }
            else {
                /* Print "ERROR: Invalid commands" */
                continue;
            }

            if (cmdret == -1) {
                /* Print "ERROR: The command is executed abnormally" */
                write(STDOUT_FILENO, _err_invalid, strlen(_err_invalid));
            }
        }

    } _BREAK:
    return 0;
}

int ls(char* dir_path, char* option) {
    int fd = -1, mode = 0;
    char path[MAX_PATH] = ".";
    if (dir_path != NULL && dir_path[0] != '-') {
        strcpy(path, dir_path);
        if (!strcmp(option, "-al")) mode = 1;
    }
    else {
        if(!strcmp(dir_path, "-al")) mode = 1;
    }
    
    if ((fd = open(path, O_RDONLY) < 0)) return -1;
    printf("fd is %d\n", fd);
    char str[10000] = "";
    read(fd, str, sizeof(str));
    write(STDOUT_FILENO, str, strlen(str));
    
    return 0;
}

int head(char* file_path, char* line) {
    return 0;
}

int tail(char* file_path, char* line) {
    return 0;
}

int mv(char* file_path1, char* file_path2) {

    return 0;
}

int mk_dir(char* dir_path) {
    return 0;
}

int rm_dir(char* dir_path) {
    return 0;
}