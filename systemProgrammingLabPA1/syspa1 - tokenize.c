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
#define MAX_CMD (1000)
#include <stdio.h> //<< Please delete.

int ls(char* dir_path, char* option);
int head(char* file_path, char* line);
int tail(char* file_path, char* line);
int mv(char* file_path1, char* file_path2);
int mk_dir(char* dir_path);
int rm_dir(char* dir_path);

char buffer[MAX_BUF];

int main() {
    int i, cmdret = 0;
    int argSize, cmdSize;
    char* commands[MAX_CMD];
    char* arguments[MAX_CMD];
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
            arguments[(argSize = 0)] = strtok(commands[i], " ");
            while ((arguments[++argSize] = strtok(NULL, " ")) != NULL);
            
            for (int j = 0; j < argSize; j++) {
                printf("[%s] ", arguments[j]);
            }
            printf("i[%d]th : argsize : %d\n", i, argSize);

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
                break;
            }
            else {
                /* Print "ERROR: Invalid commands" */

                continue;
            }

            if (cmdret == -1) {
                /* Print "ERROR: The command is executed abnormally" */
            }
        }

    }
    return 0;
}

int ls(char* dir_path, char* option) {
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