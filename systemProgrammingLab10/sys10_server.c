#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#define MAXBUF (10000)
#define DEFAULT_PORT (8112)
#define write_lt(literal) (write(STDOUT_FILENO, literal, sizeof(literal)))

char* myitos(char* dest, const int num) {
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
int main(int argc, char* argv[]) {
    char buffer[MAXBUF];
    int bufsize;
    int socket_fd;
    struct sockaddr_in saddr = { 0 };
    const int port = argc > 1 ? atoi(argv[1]) : DEFAULT_PORT;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        write_lt("socket() failed.\n");
        exit(1);
    }
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);
    if (bind(socket_fd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        write_lt("bind() failed.\n");
        exit(1);
    }
    if (listen(socket_fd, 5) < 0) {
        write_lt("listen() failed.\n");
        exit(1);
    }
    while (1) {
        char filename[256];
        int file_fd;
        int file_size;
        char file_size_str[256];
        struct sockaddr_in caddr;
        socklen_t caddr_len = sizeof(caddr);
        int accepted_fd;

        memset(filename, 0, sizeof(filename));
        memset(file_size_str, 0, sizeof(file_size_str));
        if ((accepted_fd = accept(socket_fd, (struct sockaddr*)&caddr, &caddr_len)) < 0) {
            write_lt("accept() failed\n");
            continue;
        }
        while ((bufsize = read(accepted_fd, buffer, MAXBUF)) > 0) {
            if (strchr(buffer, '\n') != NULL) *strchr(buffer, '\n') = '\0';
            else buffer[bufsize] = '\0';

            write_lt("File name: ");
            write(STDOUT_FILENO, buffer, bufsize);
            write_lt("\n");
            strcpy(filename, buffer);
            strcat(filename, "_copy");
            file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 755);
            file_size = 0;
            memset(buffer, 0, sizeof(buffer));
            while ((bufsize = read(accepted_fd, buffer, MAXBUF)) > 0) {
                write(file_fd, buffer, bufsize);
                file_size += bufsize;
            }
            myitos(file_size_str, file_size);
            write_lt("got ");
            write(STDOUT_FILENO, file_size_str, strlen(file_size_str));
            write_lt(" bytes from client.\n");
            close(file_fd);
        }
    }
    write_lt("connection terminated.\n");
    close(socket_fd);
}