#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE (16)
#define indexof(arr) (sizeof(arr) / sizeof(arr[0]))
typedef unsigned int address_t;
typedef enum _type_t { READ, WRITE, IFETCH } type_t;

int fetchAddress(address_t* pa, type_t* pt, FILE* file) {
    char buffer[BUFSIZE] = { 0 };
    if (fgets(buffer, sizeof(buffer), file) == NULL || buffer[0] == '\n' || buffer[0] == ' ') {
        return 1;
    }
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\r' || buffer[i] == '\n') {
            buffer[i] = '\0';
        }
    }
    sscanf(buffer, "%u%x", pt, pa);
    return 0;
}
//Assume that it is inclusive.
int startSimulator(FILE* trace, int const L1cacheSize, int const L2cacheSize, int const blockSize, int const associativity) {
    address_t addr;
    address_t addr_tag, addr_index, addr_offset;
    type_t type;
    if (L1cacheSize == 0 || L1cacheSize % 2 != 0) {
        printf("Wrong L1 Cache size.\n");
        exit(1);
    }
    if (L2cacheSize == 0 || L2cacheSize % 2 != 0) {
        printf("Wrong L2 Cache size.\n");
        exit(1);
    }
    if (blockSize == 0 || blockSize % 2 != 0) {
        printf("Wrong block size.\n");
        exit(1);
    }
    if (associativity == 0 || associativity % 2 != 0) {
        printf("Wrong way size.\n");
        exit(1);
    }
    fseek(trace, 0L, SEEK_SET);
    while (!fetchAddress(&addr, &type, trace)) {
        printf("%u, %x\n", type, addr);
        //비트조작부터 일어나서 합시다.
    }
    
    return 0;
}
int main(int argc, char* argv[]) {
    const int L1size[] = { 1024, 2048, 4096, 8192, 16384 };
    const int L2size[] = { 16384 };
    const int associativities[] = { 1, 2, 4, 8 };
    FILE* trace;
    if (argc < 2) {
        printf("Please input trace file path.\n");
        exit(1);
    }
    if ((trace = fopen(argv[1], "r")) == NULL) {
        printf("File opening failed.\n");
        exit(1);
    }
    printf("%d\n", indexof(L1size));
    startSimulator(trace, 1024, 16384, 16, 1);
    // startSimulator(trace, 64);
    fclose(trace);
}