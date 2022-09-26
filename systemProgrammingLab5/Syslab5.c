#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
int main() {
    char buffer[30000];
    char* args[15000];
    int argsSize;
    pid_t pid = -1;
    int num = 1111;
    while (1) {
        if(pid != -1) printf("pid [%d][num:%d] waiting input.\n", pid, num);
        fgets(buffer, sizeof(buffer), stdin);
        if(pid != -1) printf("pid [%d][num:%d] complete input.\n", pid, num);
        *(strrchr(buffer, '\n')) = '\0';
        args[(argsSize = 0)] = strtok(buffer, " ");
        while ((args[++argsSize] = strtok(NULL, " ")) != NULL); 
        if(!strcmp(args[0], "exit")){
            printf("Exiting, pid : %d.\n", pid);
            break;
        }
        if(pid != -1) printf("pid [%d] starting fork...\n", pid);
        if ((pid = fork()) == 0) {
            printf("hello from child\n");
            num += 1;
        } 
        else {
            printf("hello from parent[%d]\n", pid);
            // wait(NULL);
            num += 100;
        }
        printf("my pid is %d, and the num is %d\n", pid, num);
        


    }
}