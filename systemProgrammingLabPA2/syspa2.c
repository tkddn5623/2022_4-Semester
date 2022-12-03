#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE (20000)
#define write_lt(literal) (write(STDOUT_FILENO, literal, sizeof(literal)))

typedef struct _query {
    int user;
    int action;
    int seat;
} query;

int main(int argc, char* argv[]) {
    struct sockaddr_in server_addr;
    int listenfd;
    int port;

    if (argc < 2 || (port = atoi(argv[1])) == 0) {
        write_lt("Wrong argument. Input port num.\n");
        exit(1);
    }
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        write_lt("socket() failed.\n");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        write_lt("bind() failed.\n");
        exit(1);
    }
    if (listen(listenfd, 5) == -1) {
        write_lt("listen() failed.\n");
        exit(1);
    }
}