#include <unistd.h>
#include <sys/msg.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#define MAXBUF (1024)

typedef struct _msgbuf_t {
    long msgtype;
    char text[MAXBUF];
} msgbuf_t;

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
int runchat(const key_t key1, const key_t key2) {
    const int qid[2] = { msgget(key1, IPC_CREAT | 0666), msgget(key2, IPC_CREAT | 0666) };
    char buffer[MAXBUF] = { 0 };
    msgbuf_t msgbuf_r;
    msgbuf_t msgbuf_w;
    if (qid[0] < 0 || qid[1] < 0) return -1;
    if (fork() == 0) {
        while (1) {
            time_t t;
            memset(buffer, 0, sizeof(buffer));
            msgrcv(qid[0], &msgbuf_r, sizeof(msgbuf_t), 0, 0);
            time(&t);
            printf("%s\n", msgbuf_r.text);
            if (msgbuf_r.msgtype == 1) {
                myitos(buffer, key1);
                strcat(buffer, " read message at:\t");
                strcat(buffer, ctime(&t));
                if (strchr(buffer, '\n') != NULL) *strchr(buffer, '\n') = '\0';
                strcpy(msgbuf_w.text, buffer);

                msgbuf_w.msgtype = 2;
                msgsnd(qid[1], &msgbuf_w, sizeof(msgbuf_t), 0);
            }
        }
    }
    else {
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer), stdin);
            if (strchr(buffer, '\n') != NULL) *strchr(buffer, '\n') = '\0';
            strcpy(msgbuf_w.text, "RECEIVED ");
            strcat(msgbuf_w.text, buffer);
            msgbuf_w.msgtype = 1;
            msgsnd(qid[1], &msgbuf_w, sizeof(msgbuf_t), 0);
        }
    }
    return 0;
}
int main() {
    key_t key[2];
    printf("my id is\n");
    scanf("%d", &key[0]);
    printf("who to send ?\n");
    scanf("%d", &key[1]);
    runchat(key[0], key[1]);
}