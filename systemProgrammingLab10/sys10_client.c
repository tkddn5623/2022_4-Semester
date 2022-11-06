#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#define MAXBUF (10000)
#define DEFAULT_HOST ("127.0.0.1")
#define DEFAULT_PORT (8112)
#define write_lt(literal) (write(STDOUT_FILENO, literal, sizeof(literal)))

char* tokenize(char* const str) {
    char* token;
    char* right;
    if ((token = strchr(str, '\"')) != NULL) {
        if ((right = strchr(token + 1, '\"')) != NULL) {
            *(right + 1) = '\0';
            return token;
        }
    }
    if ((token = strchr(str, '\'')) != NULL) {
        if ((right = strchr(token + 1, '\'')) != NULL) {
            *(right + 1) = '\0';
            return token;
        }
    }
    token = strtok(str, " ");
    return token != NULL ? token : "";
}
int main(int argc, char* argv[]) {
    char buffer[MAXBUF];
    int bufsize;
    int socket_fd;
    struct hostent* hent;
    struct sockaddr_in saddr = { 0 };
    const char* hostname = argc > 1 ? argv[1] : DEFAULT_HOST;
    const int port = argc > 2 ? atoi(argv[2]) : DEFAULT_PORT;
    if (argc > 1 && !strcmp(argv[1], "localhost")) {
        hostname = DEFAULT_HOST;
    }
    
    if ((hent = gethostbyname(hostname)) == NULL) {
        write_lt("invalid hostname\n");
        exit(1);
    }
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(hostname);
    saddr.sin_port = htons(port);

    while (1) {
        if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            write_lt("socket() failed.\n");
            exit(1);
        }
        if (connect(socket_fd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
            write_lt("connect() failed. \n");
            exit(1);
        }
        int file_fd;
        char* filename;
        memset(buffer, 0, sizeof(buffer));
        if ((bufsize = read(STDIN_FILENO, buffer, MAXBUF)) <= 0) break;
        if (strchr(buffer, '\n') != NULL) *strchr(buffer, '\n') = '\0';
        filename = tokenize(buffer);
        if (!strcmp(filename, "quit")) {
            close(socket_fd);
            break;
        }
        if (!strcmp(filename, "")) continue;
        if ((file_fd = open(filename, O_RDONLY)) < 0) continue;
        write(socket_fd, filename, strlen(filename));

        memset(buffer, 0, sizeof(buffer));
        while ((bufsize = read(file_fd, buffer, sizeof(buffer))) > 0) {
            write(socket_fd, buffer, bufsize);
        }
        write(socket_fd, NULL, 0);
        close(file_fd);
        close(socket_fd);
    }
}

