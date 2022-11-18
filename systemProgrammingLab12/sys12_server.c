#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXUSER (100)
#define BUFSIZE (20000)
#define DEFAULT_PORT (8271)
#define write_lt(literal) (write(STDOUT_FILENO, literal, sizeof(literal)))

int main(int argc, char* argv[]) {
    fd_set readset, copyset;
    struct sockaddr_in server_addr;
    char buffer[BUFSIZE];
    char msg[BUFSIZE + BUFSIZE / 10];
    char name[MAXUSER][BUFSIZE / 100] = { 0 };
    int buffer_len, listenfd, fdnum, fdmax, currentusers;
    int visitedfd[1000] = { 0 };
    const int port = argc > 1 ? atoi(argv[1]) : DEFAULT_PORT;
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        write_lt("socket() failed.\n");
        exit(1);
    }
    fdmax = listenfd;
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
    FD_ZERO(&readset);
    // FD_SET(STDIN_FILENO, &readset);
    FD_SET(listenfd, &readset);
    currentusers = 0;
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        struct timeval timeout;
        int acceptedfd;
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
                if (i == listenfd) {
                    if ((acceptedfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len)) == -1) {
                        write_lt("accept() failed.\n");
                        continue;
                    }
                    FD_SET(acceptedfd, &readset);
                    if (fdmax < acceptedfd) fdmax = acceptedfd;
                }
                else {
                    if ((buffer_len = read(i, buffer, sizeof(buffer))) > 0) {
                        if (!visitedfd[i]) {
                            visitedfd[i] = 1;
                            strcpy(name[i], buffer);
                            currentusers++;
                            sprintf(msg, "%s joined. %d current users\n", name[i], currentusers);
                        }
                        else {
                            printf("got %d bytes from user %s\n", buffer_len, name[i]);
                            sprintf(msg, "%s:%s", name[i], buffer);
                        }
                        for (int j = 3; j < fdmax + 1; j++) {
                            if (j == i || j == listenfd || !visitedfd[j]) continue;
                            write(j, msg, strlen(msg));
                        }

                    }
                    else {
                        FD_CLR(i, &readset);
                        close(i);
                        currentusers--;
                        sprintf(msg, "%s leaved. %d current users\n", name[i], currentusers);
                        for (int j = 3; j < fdmax + 1; j++) {
                            if (j == listenfd) continue;
                            write(j, msg, strlen(msg));
                        }
                    }
                }
            }
        }
    }
    
}