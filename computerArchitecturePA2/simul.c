#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define BUFSIZE (16)
#define BITS (32)
#define L1CACHEMAX (16384)
#define L2CACHEMAX (16384)
#define indexof(arr) (sizeof(arr) / sizeof(arr[0]))
typedef unsigned int address_t;
typedef enum _type_t { READ, WRITE, IFETCH } type_t;
typedef enum _mlcpolicy_t { INCLUSIVE, EXCLUSIVE } mlcpolicy_t;
typedef struct _Cache {
    int valid;
    int dirty;
    int modified_time;
    address_t tag;
} Cache;
int getlog2(unsigned int _A) {
    int i = 0;
    if (_A != 1 && (_A == 0 || _A % 2 != 0)) {
        printf("getlog2 function gets wrong argument.\n");
        exit(1);
    }
    while (!(_A & 1)) {
        _A >>= 1;
        i++;
    }
    return i;
}
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
Cache* cacheInit(Cache* pcache, int const size) {
    for (int i = 0; i < size; i++) {
        pcache[i].valid = 0;
        pcache[i].dirty = 0;
        pcache[i].modified_time = -1;
        pcache[i].tag = 0;
    }
    return pcache;
}
//Assume that it is inclusive.

int startSimulator(FILE* trace, int const L1cacheSize, int const L2cacheSize, int const blockSize, int const associativity, mlcpolicy_t const mlcpolicy) {
    Cache cache_L1_D[L1CACHEMAX], cache_L1_I[L1CACHEMAX], cache_L2[L2CACHEMAX];
    address_t address;
    address_t addr_tag_L1, addr_index_L1, addr_offset_L1;
    address_t addr_tag_L2, addr_index_L2, addr_offset_L2;
    type_t type;
    int readline = 0;
    int L1D_all = 0, L1D_miss = 0, L1D_mwrite = 0;
    int L1I_all = 0, L1I_miss = 0;
    int L2_all = 0, L2_miss = 0;
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
    if (associativity != 1 && (associativity == 0 || associativity % 2 != 0)) {
        printf("Wrong way size.\n");
        exit(1);
    }
    const int isDirectmap = associativity == 1;
    const int bits_offset_L1 = getlog2(blockSize);
    const int bits_offset_L2 = bits_offset_L1;
    const int bits_index_L1 = getlog2(L1cacheSize);
    const int bits_index_L2 = getlog2(L2cacheSize);
    const int bits_tag_L1 = BITS - bits_index_L1 - bits_offset_L1;
    const int bits_tag_L2 = BITS - bits_index_L2 - bits_offset_L2;
    cacheInit(cache_L1_D, L1cacheSize);
    cacheInit(cache_L1_I, L1cacheSize);
    cacheInit(cache_L2, L2cacheSize);
    fseek(trace, 0L, SEEK_SET);
    
    while (!fetchAddress(&address, &type, trace)) {
        int isL1miss = 1;
        int setNumber, first_index_of_set, boundary_of_set;
        int LRU_index, LRU_time = INT_MAX;
        address_t copy = address;
        addr_offset_L1 = copy & ((1 << bits_offset_L1) - 1);
        copy >>= bits_offset_L1;
        addr_index_L1 = copy & ((1 << bits_index_L1) - 1);
        copy >>= bits_index_L1;
        addr_tag_L1 = copy;

        /*copy = address;
        addr_offset_L2 = copy & ((1 << bits_offset_L2) - 1);
        copy >>= bits_offset_L2;
        addr_index_L2 = copy & ((1 << bits_index_L2) - 1);
        copy >>= bits_index_L2;
        addr_tag_L2 = copy;*/

        setNumber = addr_index_L1 / associativity;
        first_index_of_set = setNumber * associativity;
        boundary_of_set = (setNumber + 1) * associativity;
        
        switch (type) {
        case READ:
        case WRITE:
            L1D_all++;
            for (int i = first_index_of_set; i < boundary_of_set; i++) {
                Cache c = cache_L1_D[i];
                if (LRU_time >= c.modified_time) {
                    LRU_time = c.modified_time;
                    LRU_index = i;
                }
                if (c.valid && c.tag == addr_tag_L1) {
                    isL1miss = 0;
                    break;
                }
            }
            break;
        case IFETCH:
            L1I_all++;
            for (int i = first_index_of_set; i < boundary_of_set; i++) {
                Cache c = cache_L1_I[i];
                if (LRU_time >= c.modified_time) {
                    LRU_time = c.modified_time;
                    LRU_index = i;
                }
                if (c.valid && c.tag == addr_tag_L1) {
                    isL1miss = 0;
                    break;
                }
            }
            break;
        default:
            printf("Invalid command type.\n");
            exit(1);
        }
        
        if (!isL1miss) continue;
        switch (type) {
        case READ:
            L1D_miss++;
            if (cache_L1_D[LRU_index].valid && cache_L1_D[LRU_index].dirty) {
                L1D_mwrite++;
                //Question : Inclusive VS Exclusive??
                //Question : DWay miss ratio vs 8way miss ratio??
                //내일할거: L1마무리, L2제작, inclusive 분리
            }
            cache_L1_D[LRU_index].valid = 1;
            cache_L1_D[LRU_index].dirty = 0;
            cache_L1_D[LRU_index].modified_time = readline;
            cache_L1_D[LRU_index].tag = addr_tag_L1;

            
            break;
        case WRITE:
            L1D_miss++;
            break;
        case IFETCH:
            L1I_miss++;
            break;
        default:
            printf("Invalid command type.\n");
            exit(1);
        }
        readline++;
    }
    return 0;
}
int main(int argc, char* argv[]) {
    const int L1size[] = { 1024, 2048, 4096, 8192, 16384 };
    const int L2size[] = { 16384 };
    const int blocksize[] = { 16, 64 };
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
    startSimulator(trace, 1024, 16384, 16, 4, INCLUSIVE);
    // startSimulator(trace, 64);
    fclose(trace);
}