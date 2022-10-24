#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h> 
int heartbeat = 0;
void handler(int sig)
{
    if(sig == SIGINT) {
        int count = heartbeat;
        for (count = 0; count < 5; count++) {
            printf("BEEP\n");
            heartbeat++;
            sleep(1);
        }
    }
}
int main() {
    signal(SIGINT, handler);
    while(heartbeat < 5) {
        sleep(2);
    }
    printf("I'm Alive!\n");
}