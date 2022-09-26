// #include <unistd.h>
#include <stdio.h>
#include <string.h>
int main() {
    char buffer[30000];
    char* args[15000];
    int argsIdx = 0;
    while(1) {
        fgets(buffer, sizeof(buffer), stdin);
        // printf("[%s]\n", buffer);
        *(strrchr(buffer, '\n')) = '\0';
        args[argsIdx++] = strtok(buffer, " ");
        while(strtok(NULL, " ")){
            
        }
        
    }
    
}