#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSEAT (256)
#define MAXUSER (1024)
#define MAXCLIENT (1536)
typedef struct _query {
    int user;
    int action;
    int seat;
} query;
struct _user_t {
    int signup;
    int passcode;
    int* address;
} User[MAXUSER] = { 0 };
int Seat[MAXSEAT];
int thread_occupied[MAXCLIENT] = { 0 };
pthread_mutex_t lock_seat = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_user = PTHREAD_MUTEX_INITIALIZER;

void* thread_io(void* _arg) {
    int const fd = (int)((int**)_arg)[0];           //Argument1 : client fd
    int* const occupied_pointer = ((int**)_arg)[1]; //Argument2 : &thread_occupied[]
    query q;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int userid = -1;
    free(_arg);                                     //Argument dynamic alloc freed.
    getpeername(fd, (struct sockaddr*)&addr, &addr_len);
    
    while (read(fd, &q, sizeof(q)) > 0) {
        int value = -1;
        if (!(q.user | q.action | q.seat)) {
            //Termination query
            pthread_mutex_lock(&lock_seat);
            write(fd, Seat, sizeof(Seat));
            pthread_mutex_unlock(&lock_seat);
            continue;
        }
        else if (q.action < 1 || q.action > 5) {
            //action field represents action id, ranging from 1 to 5 (0 to terminate)
            value = -1;
            write(fd, &value, sizeof(value));
            continue;
        }
        else {
            if ((userid != q.user && q.action != 1) || (userid != -1 && q.action == 1)) {
                //If the user tries to take this action before logging in, the action will fail
                //If the user tries to log in without logging out, the action will fail. 
                value = -1;
                write(fd, &value, sizeof(value));
            }
            else {
                switch (q.action) {
                case 1: //Log in
                    pthread_mutex_lock(&lock_user);
                    if (User[q.user].signup == 0) {
                        User[q.user].signup = 1;
                        User[q.user].address = occupied_pointer;
                        User[q.user].passcode = q.seat;
                        pthread_mutex_unlock(&lock_user);
                        userid = q.user;
                        value = 1;
                        write(fd, &value, sizeof(value));
                    }
                    else {
                        if (User[q.user].passcode != q.seat || User[q.user].address != NULL) {
                            //If already registered user tries to log in with the wrong passcode, the action will fail.
                            //If the user trying to log in is currently logged in at the other client, the action will fail. 
                            pthread_mutex_unlock(&lock_user);
                            value = -1;
                            write(fd, &value, sizeof(value));
                        }
                        else {
                            User[q.user].address = occupied_pointer;
                            pthread_mutex_unlock(&lock_user);
                            userid = q.user;
                            value = 1;
                            write(fd, &value, sizeof(value));
                        }
                    }
                    break;
                case 2:
                    if (q.seat < 0 || q.seat >= MAXSEAT) {
                        //The seat number in data field ranges from 0 to 255. The action will fail if the seat number is out of range.
                        value = -1;
                        write(fd, &value, sizeof(value));
                    }
                    else {
                        pthread_mutex_lock(&lock_seat);
                        if (Seat[q.seat] != -1) {
                            //If the seat requested by the user is already reserved by the other user, the action will fail.
                            pthread_mutex_unlock(&lock_seat);
                            value = -1;
                            write(fd, &value, sizeof(value));
                        }
                        else {
                            int fail = 0;
                            for (int i = 0; i < MAXSEAT; i++) {
                                if (Seat[i] == userid) {
                                    pthread_mutex_unlock(&lock_seat);
                                    value = -1;
                                    fail = 1;
                                    write(fd, &value, sizeof(value));
                                    break;
                                }
                            }
                            if (!fail) {
                                Seat[q.seat] = userid;
                                pthread_mutex_unlock(&lock_seat);
                                value = q.seat;
                                write(fd, &value, sizeof(value));
                            }
                        }
                    }
                    break;
                case 3:
                    pthread_mutex_lock(&lock_seat);
                    for (int i = 0; i < MAXSEAT; i++) {
                        if (Seat[i] == userid) {
                            pthread_mutex_unlock(&lock_seat);
                            value = i;
                            write(fd, &value, sizeof(value));
                            break;
                        }
                    }
                    if (value == -1) {
                        // If the user did not reserve any seat, the action will fail. 
                        pthread_mutex_unlock(&lock_seat);
                        value = -1;
                        write(fd, &value, sizeof(value));
                    }
                    break;
                case 4:
                    pthread_mutex_lock(&lock_seat);
                    for (int i = 0; i < MAXSEAT; i++) {
                        if (Seat[i] == userid) {
                            Seat[i] = -1;
                            pthread_mutex_unlock(&lock_seat);
                            value = i;
                            write(fd, &value, sizeof(value));
                            break;
                        }
                    }
                    /*if ((0 <= q.seat && q.seat < MAXSEAT) && Seat[q.seat] == userid) {
                        //The seat number in data field ranges from 0 to 255. The action will fail if the seat number is out of range.
                        Seat[q.seat] = -1;
                        pthread_mutex_unlock(&lock_seat);
                        value = q.seat;
                        write(fd, &value, sizeof(value));
                    }*/
                    if (value == -1) {
                        // If the user did not reserve any seat, the action will fail. 
                        pthread_mutex_unlock(&lock_seat);
                        value = -1;
                        write(fd, &value, sizeof(value));
                    }
                    break;
                case 5:
                    //Handling about taking this action before logging in is in LINE 54.
                    pthread_mutex_lock(&lock_user);
                    User[userid].address = NULL;
                    pthread_mutex_unlock(&lock_user);
                    userid = -1;
                    value = 1;
                    write(fd, &value, sizeof(value));
                    break;
                }
            }

        }
    }
    if (userid != -1) {
        //When thread terminates, it is assumed that LOGOUT(Action 5) was executed.
        pthread_mutex_lock(&lock_user);
        User[userid].address = NULL;
        pthread_mutex_unlock(&lock_user);
    }
    *occupied_pointer = 0; 
    close(fd);
    pthread_exit(NULL);
}
int main(int argc, char* argv[]) {
    struct sockaddr_in server_addr = { 0 };
    pthread_t tid[MAXCLIENT] = { 0 };
    int listenfd;
    int port;
    for (int i = 0; i < sizeof(Seat) / sizeof(Seat[0]); i++) {
        Seat[i] = -1;
    }
    
    if (argc <= 1 || (port = atoi(argv[1])) == 0) {
        printf("Wrong argument. argv[1] : port num.\n");
        exit(1);
    }
    for (int i = 0; argv[1][i] != '\0'; i++) {
        if (argv[1][i] < '0' || argv[1][i] > '9') {
            printf("Wrong argument. argv[1] should be only decimal num.\n");
            exit(1);
        }
    }
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("socket() failed. port: %d.\n", port);
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        printf("bind() failed. port: %d.\n", port);
        exit(1);
    }
    if (listen(listenfd, 5) == -1) {
        printf("listen() failed. port: %d.\n", port);
        exit(1);
    }
    while (1) {
        int clientfd;
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        clientfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (clientfd == -1) {
            printf("accpet() error.\n");
            continue;
        }
        else {
            int createfail = 1;
            for (int i = 0; i < MAXCLIENT; i++) {
                if (!thread_occupied[i]) {
                    thread_occupied[i] = 1;
                    void** arglist = calloc(2, sizeof(void*));
                    arglist[0] = (void*)clientfd;
                    arglist[1] = &thread_occupied[i];
                    if (pthread_create(&tid[i], NULL, thread_io, (void*)arglist)) {
                        // Here is the CORE of this program.
                        printf("pthread_creat() error.\n");
                        exit(1);
                    }
                    pthread_detach(tid[i]);
                    createfail = 0;
                    break;
                }
            }
            if (createfail) {
                printf("client number overflows.\n"); // To many client: Service refused.
                close(clientfd);
            }
        }
        

    }
}