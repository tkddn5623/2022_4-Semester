#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE (20000)
#define DEFAULT_HOST ("localhost")
#define DEFAULT_PORT (8271)
#define write_lt(literal) (write(STDOUT_FILENO, literal, sizeof(literal)))

int main(int argc, char* argv[]) {
    fd_set readset, copyset;
    struct hostent* h;
    struct sockaddr_in saddr;
    char name[BUFSIZE / 10] = { 0 };
    char buffer[BUFSIZE];
    int buffer_len;
    int cfd, fdnum, fdmax;
    const char* hostname = argc > 1 ? argv[1] : DEFAULT_HOST;
    const int port = argc > 2 ? atoi(argv[2]) : DEFAULT_PORT;

    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        write_lt("socket() failed.\n");
        exit(1);
    }
    if ((h = gethostbyname(hostname)) == NULL) {
        printf("invalid hostname %s\n", hostname);
        exit(1);
    }
    saddr.sin_family = AF_INET;
    memcpy(&saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    saddr.sin_port = htons(port);
    if (connect(cfd, (struct sockaddr*)&saddr, sizeof(saddr)) == -1) {
        write_lt("connect() failed.\n");
        exit(1);
    }
    FD_ZERO(&readset);
    FD_SET(STDIN_FILENO, &readset);
    FD_SET(cfd, &readset);
    fdmax = cfd + 1;
    write_lt("Insert your name :");
    name[read(STDIN_FILENO, name, sizeof(name))] = '\0';
    if (strchr(name, '\n') != NULL) *strchr(name, '\n') = '\0';
    write(cfd, name, strlen(name));
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        struct timeval timeout;
        copyset = readset;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        if ((fdnum = select(fdmax + 1, &copyset, NULL, NULL, &timeout)) == -1) {
            write_lt("select() failed.\n");
            exit(1);
        }
        else if (fdnum == 0) {
            // write_lt("Time out.\n");
            continue;
        }
        for (int i = 0; i < fdmax + 1; i++) {
            if (FD_ISSET(i, &copyset)) {
                if (i == STDIN_FILENO) {
                    if ((buffer_len = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
                        if (!strcmp(buffer, "quit") || !strcmp(buffer, "quit\n")) return 0;
                        write(cfd, buffer, buffer_len);
                    }
                }
                else {
                    if ((buffer_len = read(i, buffer, sizeof(buffer))) > 0) {
                        write(STDOUT_FILENO, buffer, buffer_len);
                    }
                }
            }

        }
    }

}