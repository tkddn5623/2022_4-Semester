#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

typedef struct _query {
    int user;
    int action;
    int seat;
} query;

int main (int argc, char *argv[]) {
    int client_socket = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    if (argc < 3) {
	printf("argv[1]: server address, argv[2]: port number\n");
	exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(client_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
	printf("Connection failed\n");
	exit(1);
    }
    //printf("Connected to server! Now querying...\n");

    while (1) {
	query q;
	scanf("%d %d %d", &q.user, &q.action, &q.seat);
	printf("%d %d %d\n", q.user, q.action, q.seat);
	send(client_socket, &q, sizeof(q), 0);
	int ret;
	if (!(q.user | q.action | q.seat))
	{
	    int arr[256];
	    recv(client_socket, arr, sizeof(arr), 0);
	    int i;
	    for(i=0;i<=255;i++) printf("%d ", arr[i]);
	    printf("\n");
	    break;
	}
	else{
	    recv(client_socket, &ret, sizeof(ret), 0);
	    printf("%d\n", ret);
	}
    }

}
