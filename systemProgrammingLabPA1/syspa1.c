#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#define MAX_BUF (50000)
#define MAX_PATH (5000)
#define MAX_CMD (1000)
#define MAX_ITOS (100)

int ls(char* dir_path, char* option);
int head(char* file_path, char* line_arg);
int tail(char* file_path, char* line_arg);
int mv(char* file_path1, char* file_path2);
int mk_dir(char* dir_path);
int rm_dir(char* dir_path);
char* Myitos(char* dest, const int num);
int Mystoi(const char* str);

const char* const _err_invalid_cmd = "ERROR: Invalid commands\n";
const char* const _err_invalid_path = "ERROR: invalid path\n";
const char* const _err_abnormal = "ERROR: The command is executed abnormally\n";
const char* const _err_rmdir_notempty[2] = { "rmdir: failed to remove '" , "': Directory is not empty\n" };

int main() {
    int cmdret = 0;
    int argSize, cmdSize;
    char buffer[MAX_BUF];
    char* commands[MAX_CMD];
    char* arguments[MAX_CMD];
    while (1) {
        /* Input commands */
        memset(buffer, 0, sizeof(buffer));
        if (read(STDIN_FILENO, buffer, sizeof(buffer)) <= 0) break;
        if ((commands[(cmdSize = 0)] = strtok(buffer, "\n")) == NULL) continue;
        while ((commands[++cmdSize] = strtok(NULL, "\n")) != NULL);
        
        /* ============== */
        for (int i = 0; i < cmdSize; i++) {
            /* Tokenize commands (Use strtok function) */
            memset(arguments, 0, sizeof(arguments));
            if ((arguments[(argSize = 0)] = strtok(commands[i], " ")) == NULL) continue;
            while ((arguments[++argSize] = strtok(NULL, " ")) != NULL);
            /* ======================================= */
            /* Command */
            if (!strcmp("ls", arguments[0])) {
                cmdret = ls(arguments[1], arguments[2]);
            }
            else if (!strcmp("head", arguments[0])) {
                cmdret = head(arguments[3], arguments[2]);
            }
            else if (!strcmp("tail", arguments[0])) {
                cmdret = tail(arguments[3], arguments[2]);
            }
            else if (!strcmp("mv", arguments[0])) {
                cmdret = mv(arguments[1], arguments[2]);
            }
            else if (!strcmp("mkdir", arguments[0])) {
                cmdret = mk_dir(arguments[1]);
            }
            else if (!strcmp("rmdir", arguments[0])) {
                cmdret = rm_dir(arguments[1]);
            }
            else if (!strcmp("quit", arguments[0])) {
                goto _BREAK;
            }
            else {
                /* Print "ERROR: Invalid commands" */
                write(STDOUT_FILENO, _err_invalid_cmd, strlen(_err_invalid_cmd));
                continue;
            }

            if (cmdret == -1) {
                /* Print "ERROR: The command is executed abnormally" */
                if (errno == ENOENT) write(STDOUT_FILENO, _err_invalid_path, strlen(_err_invalid_path));
                write(STDOUT_FILENO, _err_abnormal, strlen(_err_abnormal));
            }
            /* ======= */
        }

    } _BREAK:
    return 0;
}
int ls(char* dir_path, char* option) {
    DIR* dir;
    struct dirent* dr;
    int mode = 0;
    int entSize = 0;
    char path[MAX_PATH] = ".";
    if (dir_path == NULL || dir_path[0] == '-') return -1;
    else strcpy(path, dir_path);

    if (option != NULL && !strcmp(option, "-al")) mode = 1;
    if ((dir = opendir(dir_path)) == NULL) return -1;

    switch (mode) {
    case 0:
        while ((dr = readdir(dir)) != NULL) {
            if (strcmp(dr->d_name, ".") && strcmp(dr->d_name, "..")) {
                if (entSize != 0) write(STDOUT_FILENO, " ", 1);
                write(STDOUT_FILENO, dr->d_name, strlen(dr->d_name));
                entSize++;
            }
        }
        break;
    case 1:
        while ((dr = readdir(dir)) != NULL) {
            struct stat st;
            struct passwd* user;
            struct group* group;
            char linkString[MAX_ITOS];
            char capacityString[MAX_ITOS];
            if (entSize != 0) write(STDOUT_FILENO, "\n", 1);
            stat(dr->d_name, &st);
            user = getpwuid(st.st_uid);
            group = getgrgid(st.st_gid);
            Myitos(linkString, st.st_nlink);
            Myitos(capacityString, st.st_size);
            const int mod = st.st_mode;
            if ((mod >> 14) & 1) write(STDOUT_FILENO, "d", 1);
            else write(STDERR_FILENO, "-", 1);
            for (int i = 8; i >= 0; i--) {
                int bit = (mod >> i) & 1;
                switch (i % 3) {
                case 2:
                    if (bit == 1) write(STDOUT_FILENO, "r", 1);
                    else write(STDOUT_FILENO, "-", 1);
                    break;
                case 1:
                    if (bit == 1) write(STDOUT_FILENO, "w", 1);
                    else write(STDOUT_FILENO, "-", 1);
                    break;
                case 0:
                    if (bit == 1) write(STDOUT_FILENO, "x", 1);
                    else write(STDOUT_FILENO, "-", 1);
                    break;
                }
            }
            write(STDOUT_FILENO, " ", 1);
            write(STDOUT_FILENO, linkString, strlen(linkString));
            write(STDOUT_FILENO, " ", 1);
            write(STDOUT_FILENO, user->pw_name, strlen(user->pw_name));
            write(STDOUT_FILENO, " ", 1);
            write(STDOUT_FILENO, group->gr_name, strlen(group->gr_name));
            write(STDOUT_FILENO, " ", 1);
            if ((mod >> 14) & 1) write(STDOUT_FILENO, "4096", strlen("4096"));
            else write(STDOUT_FILENO, capacityString, strlen(capacityString));
            write(STDOUT_FILENO, " ", 1);
            write(STDOUT_FILENO, dr->d_name, strlen(dr->d_name));
            entSize++;
        }
        break;
    }
    write(STDOUT_FILENO, "\n", 1);
    closedir(dir);
    return 0;
}
int head(char* file_path, char* line_arg) {
    char buffer[MAX_BUF] = { 0 };
    const int line_num = Mystoi(line_arg);
    int newlineCount = 0;
    int fd;
    if ((fd = open(file_path, O_RDONLY)) < 0) return -1;
    else if (line_num <= 0) return 0;

    while (newlineCount < line_num && (read(fd, buffer, sizeof(buffer)) > 0)) {
        const int bufSize = strlen(buffer);
        int strSize;
        for (strSize = 0; strSize < bufSize; strSize++) {
            const char ch = buffer[strSize];
            if (ch == '\n') newlineCount++;
            if (newlineCount >= line_num) break;
        }
        write(STDOUT_FILENO, buffer, strSize);
    }
    write(STDOUT_FILENO, "\n", 1);
    close(fd);
    return 0;
}
int tail(char* file_path, char* line_arg) {
    static char bigbuffer[MAX_BUF * 20];
    int fd, bufSize, linesSize = 0;
    const int line_num = Mystoi(line_arg);
    int* lines_itemSize = calloc(line_num + 1, sizeof(int));
    char** lines = calloc(line_num, sizeof(char*));
    
    memset(bigbuffer, 0, sizeof(bigbuffer));
    if ((fd = open(file_path, O_RDONLY)) < 0) return -1;
    
    read(fd, bigbuffer, sizeof(bigbuffer));
    bufSize = strlen(bigbuffer);
    for (int i = bufSize - 1; i >= 0; i--) {
        const char ch = bigbuffer[i];
        if (ch == '\n') {
            lines[linesSize] = &bigbuffer[i] + 1;
            if (linesSize == 0) lines_itemSize[0] = strlen(lines[0]);
            else lines_itemSize[linesSize] = (int)(lines[linesSize - 1] - lines[linesSize]);
            linesSize++;
        }
        if (linesSize >= line_num) break;
    }
    if (linesSize < line_num) {
        lines[linesSize] = bigbuffer;
        if (linesSize == 0) lines_itemSize[linesSize] = strlen(bigbuffer);
        else lines_itemSize[linesSize] = (int)(lines[linesSize - 1] - lines[linesSize]);
        linesSize++;
    }
    for (int i = linesSize - 1; i >= 0; i--) {
        write(STDOUT_FILENO, lines[i], lines_itemSize[i]);
    }
    write(STDOUT_FILENO, "\n", 1);
    free(lines_itemSize);
    free(lines);
    close(fd);
    return 0;
}
int mv(char* file_path1, char* file_path2) {
    int fd1 = -1, fd2 = -1;
    char file_destpath[MAX_PATH];
    char buffer[MAX_BUF] = { 0 };
    DIR* dir = opendir(file_path2);
    strcpy(file_destpath, file_path2);
    if (dir != NULL) {
        strcat(file_destpath, "/");
        strcat(file_destpath, file_path1);
        closedir(dir);
    }
    if ((fd1 = open(file_path1, O_RDONLY)) < 0) return -1;
    if ((fd2 = open(file_path2, O_WRONLY | O_CREAT | O_TRUNC, 755)) < 0) {
        close(fd1);
        return -1;
    }
    while ((read(fd1, buffer, sizeof(buffer))) > 0) {
        write(fd2, buffer, strlen(buffer));
    }
    close(fd1);
    close(fd2);
    return unlink(file_path1);
}
int mk_dir(char* dir_path) {
    return mkdir(dir_path, 755);
}
int rm_dir(char* dir_path) {
    if (rmdir(dir_path) != 0) {
        const int err = errno;
        if (err == ENOTEMPTY) {
            write(STDOUT_FILENO, _err_rmdir_notempty[0], strlen(_err_rmdir_notempty[0]));
            write(STDOUT_FILENO, dir_path, strlen(dir_path));
            write(STDOUT_FILENO, _err_rmdir_notempty[1], strlen(_err_rmdir_notempty[1]));
        }
        return -1;
    }
    return 0;
}
char* Myitos(char* dest, const int num) {
    int digitsSize = 0;
    int digits[10];
    for (int i = num; i > 0; i /= 10) {
        digits[digitsSize++] = i % 10;
    }
    for (int i = digitsSize - 1; i >= 0; i--) {
        dest[digitsSize - 1 - i] = digits[i] + '0';
    }
    dest[digitsSize] = '\0';
    return dest;
}
int Mystoi(const char* str) {
    const int len = strlen(str);
    int ret = 0, base = 1;
    for (int i = len - 1; i >= 0; i--) {
        const char ch = str[i];
        if (ch < '0' || ch > '9') return 0;
        else ret += base * (ch - '0');
        
        base *= 10;
    }
    return ret;
}