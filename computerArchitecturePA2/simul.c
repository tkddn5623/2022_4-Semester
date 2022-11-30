#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define BUFSIZE (16)
#define BITS (32)
#define L2WAY (8)
#define lengthof(arr) ((int)(sizeof(arr) / sizeof(arr[0])))
typedef unsigned int address_t;
typedef enum _type_t { READ, WRITE, IFETCH } type_t;
typedef enum _validbit_t { INVALID, VALID } validbit_t;
typedef enum _dirtybit_t { CLEAN, DIRTY } dirtybit_t;
typedef enum _mlcpolicy_t { INCLUSIVE, EXCLUSIVE } mlcpolicy_t;
typedef struct _Cache {
    validbit_t valid;
    dirtybit_t dirty;
    int modified_time;
    address_t tag;
} Cache;
typedef struct _Resultv {
    double missratio_I;
    double missratio_D;
    double missratio_L2;
    int mblockwrite_D;
} Resultv;
int _log2(unsigned int _A) {
    int i = 0;
    if (_A != 1 && (_A == 0 || _A % 2 != 0)) {
        printf("_log2 function gets wrong argument.\n");
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
        pcache[i].valid = INVALID;
        pcache[i].dirty = CLEAN;
        pcache[i].modified_time = -1;
        pcache[i].tag = 0;
    }
    return pcache;
}
Cache* cacheFind(Cache** pcache, address_t const address, int const bits_index, int const bits_offset, int const associativity) {
    address_t copy, tag, index;
    copy = address;

    copy >>= bits_offset;
    index = copy & ((1 << bits_index) - 1);
    copy >>= bits_index;
    tag = copy;

    for (int i = 0; i < associativity; i++) {
        if (pcache[i][index].valid == VALID && pcache[i][index].tag == tag) {
            return &pcache[i][index];
        }
    }
    return NULL;
}
Cache cacheLoadnEvict(Cache** pcache, address_t const address, int const bits_index, int const bits_offset, int const associativity, int const readline, dirtybit_t isDirty) {
    Cache evictedCache;
    address_t copy, tag, index;
    int LRU_set_index = 0, LRU_time_max = INT_MAX;
    copy = address;

    copy >>= bits_offset;
    index = copy & ((1 << bits_index) - 1);
    copy >>= bits_index;
    tag = copy;

    for (int i = 0; i < associativity; i++) {
        Cache c = pcache[i][index];
        
        if (LRU_time_max > c.modified_time) {
            LRU_time_max = c.modified_time;
            LRU_set_index = i;
        }
    }
    evictedCache = pcache[LRU_set_index][index];
    pcache[LRU_set_index][index].tag = tag;
    pcache[LRU_set_index][index].valid = VALID;
    pcache[LRU_set_index][index].dirty = isDirty;
    pcache[LRU_set_index][index].modified_time = readline;
    return evictedCache;
}


Resultv startSimulator(FILE* trace, int const L1cacheSize, int const L2cacheSize, int const blockSize, int const L1associativity, mlcpolicy_t const mlcpolicy) {
    Resultv resultv;
    Cache** cache_L1_D, ** cache_L1_I, ** cache_L2;
    address_t address;
    address_t addr_index_L1;
    address_t addr_index_L2;
    type_t type;
    int readline = 0;
    int L1D_all = 0, L1D_miss = 0;
    int L1I_all = 0, L1I_miss = 0;
    int L2_all = 0, L2_miss = 0, L2_mwrite = 0;
    const int L2associativity = L2WAY;
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
    if (L1associativity != 1 && (L1associativity == 0 || L1associativity % 2 != 0)) {
        printf("Wrong L1 way size.\n");
        exit(1);
    }
    if (L2associativity != 1 && (L2associativity == 0 || L2associativity % 2 != 0)) {
        printf("Wrong L2 way size.\n");
        exit(1);
    }
    if (mlcpolicy != INCLUSIVE && mlcpolicy != EXCLUSIVE) {
        printf("Invalid MLC policy.\n");
        exit(1);
    }
    const int L1setSize = L1cacheSize / L1associativity;
    const int L2setSize = L2cacheSize / L2associativity;
    const int bits_offset_L1 = _log2(blockSize);
    const int bits_offset_L2 = bits_offset_L1;
    const int bits_index_L1 = _log2(L1setSize);
    const int bits_index_L2 = _log2(L2setSize);
    // const int bits_tag_L1 = BITS - bits_index_L1 - bits_offset_L1;
    // const int bits_tag_L2 = BITS - bits_index_L2 - bits_offset_L2;
    cache_L1_D = calloc(L1associativity, sizeof(Cache*));
    cache_L1_I = calloc(L1associativity, sizeof(Cache*));
    cache_L2 = calloc(L2associativity, sizeof(Cache*));
    for (int i = 0; i < L1associativity; i++) {
        cache_L1_D[i] = calloc(L1setSize, sizeof(Cache));
        cache_L1_I[i] = calloc(L1setSize, sizeof(Cache));
        cacheInit(cache_L1_D[i], L1setSize);
        cacheInit(cache_L1_I[i], L1setSize);
    }
    for (int i = 0; i < L2associativity; i++) {
        cache_L2[i] = calloc(L2setSize, sizeof(Cache));
        cacheInit(cache_L2[i], L2setSize);
    }
    fseek(trace, 0L, SEEK_SET);
    
    for (readline = 0; !fetchAddress(&address, &type, trace); readline++) {
        int isL1miss = 0;
        Cache* findc = NULL;
        Cache evicted;
        address_t copy = address;
        
        copy >>= bits_offset_L1;
        addr_index_L1 = copy & ((1 << bits_index_L1) - 1);
        copy >>= bits_index_L1;
        copy = address;
        copy >>= bits_offset_L2;
        addr_index_L2 = copy & ((1 << bits_index_L2) - 1);
        copy >>= bits_index_L2;
        //L1 Cache Access Simulating...
        switch (type) {
        case READ:
            L1D_all++;
            if ((findc = cacheFind(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity)) == NULL) {
                //L1D Read miss
                isL1miss = 1;
                L1D_miss++;
            }
            else { //L1D Read Hit
                findc->modified_time = readline;
            }
            break;
        case WRITE:
            L1D_all++;
            if ((findc = cacheFind(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity)) == NULL) {
                //L1D Write miss
                isL1miss = 1;
                L1D_miss++;
            }
            else { //L1D Write Hit
                findc->modified_time = readline;
                findc->dirty = DIRTY;
                if (mlcpolicy == INCLUSIVE) { //For inclusve: set identity in L2 
                    Cache* inclusive_L2find = cacheFind(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity);
                    if (inclusive_L2find == NULL) {
                        printf("Fatal error occured: There was no data in L2 even though it was Inclusive.\n");
                        exit(1);
                    }
                    inclusive_L2find->dirty = DIRTY;
                }
            }
            break;
        case IFETCH:
            L1I_all++;
            if ((findc = cacheFind(cache_L1_I, address, bits_index_L1, bits_offset_L1, L1associativity)) == NULL) {
                //L1I fetch miss
                isL1miss = 1;
                L1I_miss++;
            }
            else { //L1I Ifetch Hit
                findc->modified_time = readline;
            }
            break;
        default:
            printf("Invalid command type.\n0:Data Read, 1:Data Write, 2:Instruction Fetch\n");
            exit(1);
        }
        
        if (!isL1miss) continue;
        //L2 Cache Access Simulating...
        if (mlcpolicy == INCLUSIVE) {
            switch (type) {
            case READ:
                L2_all++;
                if ((findc = cacheFind(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity)) == NULL) {
                    //L1 Read miss and L2 miss
                    address_t backtrack;
                    L2_miss++;
                    evicted = cacheLoadnEvict(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity, readline, CLEAN);
                    if (evicted.dirty == DIRTY) {
                        L2_mwrite++;
                    }
                    if (evicted.valid == VALID) {
                        backtrack = ((evicted.tag << bits_index_L2) | addr_index_L2) << bits_offset_L2;
                        if ((findc = cacheFind(cache_L1_D, backtrack, bits_index_L1, bits_offset_L1, L1associativity)) != NULL) {
                            findc->dirty = CLEAN;
                            findc->modified_time = -1;
                            findc->tag = 0;
                            findc->valid = INVALID;
                        }
                        else if ((findc = cacheFind(cache_L1_I, backtrack, bits_index_L1, bits_offset_L1, L1associativity)) != NULL) {
                            findc->dirty = CLEAN;
                            findc->modified_time = -1;
                            findc->tag = 0;
                            findc->valid = INVALID;
                        }
                    }
                    cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, CLEAN);
                }
                else {
                    //L1 Read miss but L2 hit
                    findc->modified_time = readline;
                    cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, findc->dirty);
                }
                break;
            case WRITE:
                L2_all++;
                if ((findc = cacheFind(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity)) == NULL) {
                    //L1 Write miss and L2 miss
                    address_t backtrack;
                    L2_miss++;
                    evicted = cacheLoadnEvict(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity, readline, DIRTY);
                    if (evicted.dirty == DIRTY) {
                        L2_mwrite++;
                    }
                    if (evicted.valid == VALID) {
                        backtrack = ((evicted.tag << bits_index_L2) | addr_index_L2) << bits_offset_L2;
                        if ((findc = cacheFind(cache_L1_D, backtrack, bits_index_L1, bits_offset_L1, L1associativity)) != NULL) {
                            findc->dirty = CLEAN;
                            findc->modified_time = -1;
                            findc->tag = 0;
                            findc->valid = INVALID;
                        }
                        else if ((findc = cacheFind(cache_L1_I, backtrack, bits_index_L1, bits_offset_L1, L1associativity)) != NULL) {
                            findc->dirty = CLEAN;
                            findc->modified_time = -1;
                            findc->tag = 0;
                            findc->valid = INVALID;
                        }
                    }
                    cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, DIRTY);
                }
                else {
                    //L1 Write miss but L2 hit
                    findc->modified_time = readline;
                    findc->dirty = DIRTY;
                    cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, DIRTY);
                }
                break;
            case IFETCH:
                L2_all++;
                if ((findc = cacheFind(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity)) == NULL) {
                    //L1 Ifetch miss and L2 miss
                    address_t backtrack;
                    L2_miss++;
                    evicted = cacheLoadnEvict(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity, readline, CLEAN);
                    if (evicted.dirty == DIRTY) {
                        L2_mwrite++;
                    }
                    if (evicted.valid == VALID) {
                        backtrack = ((evicted.tag << bits_index_L2) | addr_index_L2) << bits_offset_L2;
                        if ((findc = cacheFind(cache_L1_D, backtrack, bits_index_L1, bits_offset_L1, L1associativity)) != NULL) {
                            findc->dirty = CLEAN;
                            findc->modified_time = -1;
                            findc->tag = 0;
                            findc->valid = INVALID;
                        }
                        else if ((findc = cacheFind(cache_L1_I, backtrack, bits_index_L1, bits_offset_L1, L1associativity)) != NULL) {
                            findc->dirty = CLEAN;
                            findc->modified_time = -1;
                            findc->tag = 0;
                            findc->valid = INVALID;
                        }
                    }
                    cacheLoadnEvict(cache_L1_I, address, bits_index_L1, bits_offset_L1, L1associativity, readline, CLEAN);
                }
                else {
                    //L1 Ifetch miss but L2 hit
                    findc->modified_time = readline;
                    cacheLoadnEvict(cache_L1_I, address, bits_index_L1, bits_offset_L1, L1associativity, readline, CLEAN);
                }
                break;
            }
        }
        else if (mlcpolicy == EXCLUSIVE) {
            switch (type) {
            case READ:
                L2_all++;
                if ((findc = cacheFind(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity)) == NULL) {
                    //L1 Read miss and L2 miss
                    address_t backtrack;
                    L2_miss++;
                    evicted = cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, CLEAN);
                    if (evicted.valid == VALID) {
                        backtrack = ((evicted.tag << bits_index_L1) | addr_index_L1) << bits_offset_L1;
                        evicted = cacheLoadnEvict(cache_L2, backtrack, bits_index_L2, bits_offset_L2, L2associativity, evicted.modified_time, evicted.dirty);
                        if (evicted.dirty == DIRTY) {
                            L2_mwrite++;
                        }
                    }
                }
                else {
                    //L1 Read miss but L2 hit
                    address_t backtrack;
                    evicted = cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, findc->dirty);
                    findc->dirty = CLEAN;
                    findc->modified_time = -1;
                    findc->tag = 0;
                    findc->valid = INVALID;
                    if (evicted.valid == VALID) {
                        backtrack = ((evicted.tag << bits_index_L1) | addr_index_L1) << bits_offset_L1;
                        evicted = cacheLoadnEvict(cache_L2, backtrack, bits_index_L2, bits_offset_L2, L2associativity, evicted.modified_time, evicted.dirty);
                        if (evicted.dirty == DIRTY) {
                            L2_mwrite++;
                        }
                    }
                }
                break;
            case WRITE:
                L2_all++;
                if ((findc = cacheFind(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity)) == NULL) {
                    //L1 Write miss and L2 miss
                    address_t backtrack;
                    L2_miss++;
                    evicted = cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, DIRTY);
                    if (evicted.valid == VALID) {
                        backtrack = ((evicted.tag << bits_index_L1) | addr_index_L1) << bits_offset_L1;
                        evicted = cacheLoadnEvict(cache_L2, backtrack, bits_index_L2, bits_offset_L2, L2associativity, evicted.modified_time, evicted.dirty);
                        if (evicted.dirty == DIRTY) {
                            L2_mwrite++;
                        }
                    }
                }
                else {
                    //L1 Write miss but L2 hit
                    address_t backtrack;
                    evicted = cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, DIRTY);
                    findc->dirty = CLEAN;
                    findc->modified_time = -1;
                    findc->tag = 0;
                    findc->valid = INVALID;
                    if (evicted.valid == VALID) {
                        backtrack = ((evicted.tag << bits_index_L1) | addr_index_L1) << bits_offset_L1;
                        evicted = cacheLoadnEvict(cache_L2, backtrack, bits_index_L2, bits_offset_L2, L2associativity, evicted.modified_time, evicted.dirty);
                        if (evicted.dirty == DIRTY) {
                            L2_mwrite++;
                        }
                    }
                }
                break;
            case IFETCH:
                L2_all++;
                if ((findc = cacheFind(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity)) == NULL) {
                    //L1 Ifetch miss and L2 miss
                    address_t backtrack;
                    L2_miss++;
                    evicted = cacheLoadnEvict(cache_L1_I, address, bits_index_L1, bits_offset_L1, L1associativity, readline, CLEAN);
                    if (evicted.valid == VALID) {
                        backtrack = ((evicted.tag << bits_index_L1) | addr_index_L1) << bits_offset_L1;
                        evicted = cacheLoadnEvict(cache_L2, backtrack, bits_index_L2, bits_offset_L2, L2associativity, evicted.modified_time, evicted.dirty);
                        if (evicted.dirty == DIRTY) {
                            L2_mwrite++;
                        }
                    }
                }
                else {
                    //L1 Ifetch miss but L2 hit
                    address_t backtrack;
                    evicted = cacheLoadnEvict(cache_L1_I, address, bits_index_L1, bits_offset_L1, L1associativity, readline, CLEAN);
                    findc->dirty = CLEAN;
                    findc->modified_time = -1;
                    findc->tag = 0;
                    findc->valid = INVALID;
                    if (evicted.valid == VALID) {
                        backtrack = ((evicted.tag << bits_index_L1) | addr_index_L1) << bits_offset_L1;
                        evicted = cacheLoadnEvict(cache_L2, backtrack, bits_index_L2, bits_offset_L2, L2associativity, evicted.modified_time, evicted.dirty);
                        if (evicted.dirty == DIRTY) {
                            L2_mwrite++;
                        }
                    }
                }
                break;
            }
        }

    }

    resultv.missratio_D = (double)(L1D_miss * 100) / L1D_all;
    resultv.missratio_I = (double)(L1I_miss * 100) / L1I_all;
    resultv.missratio_L2 = (double)(L2_miss * 100) / L2_all;
    resultv.mblockwrite_D = L2_mwrite;

    for (int i = 0; i < L1associativity; i++) {
        free(cache_L1_D[i]);
        free(cache_L1_I[i]);
        free(cache_L2[i]);
    }
    free(cache_L1_D);
    free(cache_L1_I);
    free(cache_L2);
    return resultv;
}
int main(int argc, char* argv[]) {
    const int L1size[] = { 1024, 2048, 4096, 8192, 16384 };
    const int L2size[] = { 16384 };
    const int blocksize[] = { 16, 64 };
    const int L1associativities[] = { 1, 2, 4, 8 };
    Resultv result_in[lengthof(blocksize)][lengthof(L1associativities)][lengthof(L1size)] = { 0 };
    Resultv result_ex[lengthof(blocksize)][lengthof(L1associativities)][lengthof(L1size)] = { 0 };
    FILE* trace;
    if (argc < 2) {
        printf("Please input trace file path.\n");
        exit(1);
    }
    if ((trace = fopen(argv[1], "r")) == NULL) {
        printf("File opening failed.\n");
        exit(1);
    }
    printf("Please Wait! It takes less than 20 seconds.\n");
    for (int i = 0; i < lengthof(blocksize); i++) {
        for (int j = 0; j < lengthof(L1associativities); j++) {
            for (int k = 0; k < lengthof(L1size); k++) {
                result_in[i][j][k] = startSimulator(trace, L1size[k], L2size[0], blocksize[i], L1associativities[j], INCLUSIVE);
            }
        }
    }
    printf("Inclusive simulation complete.\n");
    for (int i = 0; i < lengthof(blocksize); i++) {
        for (int j = 0; j < lengthof(L1associativities); j++) {
            for (int k = 0; k < lengthof(L1size); k++) {
                result_ex[i][j][k] = startSimulator(trace, L1size[k], L2size[0], blocksize[i], L1associativities[j], EXCLUSIVE);
            }
        }
    }
    printf("Exclusive simulation complete.\n");
    fclose(trace);
    printf("Simulation Complete.\n");

    
    for (int i = 0; i < lengthof(blocksize); i++) {
        printf("\n");
        printf("L1-D Miss Ratio (block size = %dB)\n", blocksize[i]);
        printf("LRU/%d     ", blocksize[i]);
        for (int j = 0; j < lengthof(L1size); j++) {
            printf("%d     ", L1size[j]);
        }
        printf("\n");
        for (int j = 0; j < lengthof(L1associativities); j++) {
            if (L1associativities[j] == 1) printf("Direct ");
            else printf("%d-way  ", L1associativities[j]);
            for (int k = 0; k < lengthof(L1size); k++) {
                printf("%7.2f%% ", result_in[i][j][k].missratio_D);
            }
            printf("\n");
        }
    }
    for (int i = 0; i < lengthof(blocksize); i++) {
        printf("\n");
        printf("L1-I Miss Ratio (block size = %dB)\n", blocksize[i]);
        printf("LRU/%d     ", blocksize[i]);
        for (int j = 0; j < lengthof(L1size); j++) {
            printf("%d     ", L1size[j]);
        }
        printf("\n");
        for (int j = 0; j < lengthof(L1associativities); j++) {
            if (L1associativities[j] == 1) printf("Direct ");
            else printf("%d-way  ", L1associativities[j]);
            for (int k = 0; k < lengthof(L1size); k++) {
                printf("%7.2f%% ", result_in[i][j][k].missratio_I);
            }
            printf("\n");
        }
    }
    for (int i = 0; i < lengthof(blocksize); i++) {
        printf("\n");
        printf("L2 Inclusive Miss Ratio (block size = %dB)\n", blocksize[i]);
        printf("LRU/%d     ", blocksize[i]);
        for (int j = 0; j < lengthof(L1size); j++) {
            printf("%d     ", L1size[j]);
        }
        printf("\n");
        for (int j = 0; j < lengthof(L1associativities); j++) {
            if (L1associativities[j] == 1) printf("Direct ");
            else printf("%d-way  ", L1associativities[j]);
            for (int k = 0; k < lengthof(L1size); k++) {
                printf("%7.2f%% ", result_in[i][j][k].missratio_L2);
            }
            printf("\n");
        }
    }
    for (int i = 0; i < lengthof(blocksize); i++) {
        printf("\n");
        printf("L2 Exclusive Miss Ratio (block size = %dB)\n", blocksize[i]);
        printf("LRU/%d     ", blocksize[i]);
        for (int j = 0; j < lengthof(L1size); j++) {
            printf("%d     ", L1size[j]);
        }
        printf("\n");
        for (int j = 0; j < lengthof(L1associativities); j++) {
            if (L1associativities[j] == 1) printf("Direct ");
            else printf("%d-way  ", L1associativities[j]);
            for (int k = 0; k < lengthof(L1size); k++) {
                printf("%7.2f%% ", result_ex[i][j][k].missratio_L2);
            }
            printf("\n");
        }
    }
    for (int i = 0; i < lengthof(blocksize); i++) {
        printf("\n");
        printf("Number of Memory Block Writes (Inclusive) (block size = %dB)\n", blocksize[i]);
        printf("LRU/%d     ", blocksize[i]);
        for (int j = 0; j < lengthof(L1size); j++) {
            printf("%d     ", L1size[j]);
        }
        printf("\n");
        for (int j = 0; j < lengthof(L1associativities); j++) {
            if (L1associativities[j] == 1) printf("Direct ");
            else printf("%d-way  ", L1associativities[j]);
            for (int k = 0; k < lengthof(L1size); k++) {
                printf("%8d ", result_in[i][j][k].mblockwrite_D);
            }
            printf("\n");
        }
    }
    for (int i = 0; i < lengthof(blocksize); i++) {
        printf("\n");
        printf("Number of Memory Block Writes (Exclusive) (block size = %dB)\n", blocksize[i]);
        printf("LRU/%d     ", blocksize[i]);
        for (int j = 0; j < lengthof(L1size); j++) {
            printf("%d     ", L1size[j]);
        }
        printf("\n");
        for (int j = 0; j < lengthof(L1associativities); j++) {
            if (L1associativities[j] == 1) printf("Direct ");
            else printf("%d-way  ", L1associativities[j]);
            for (int k = 0; k < lengthof(L1size); k++) {
                printf("%8d ", result_ex[i][j][k].mblockwrite_D);
            }
            printf("\n");
        }
    }
}