#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define BUFSIZE (16)
#define BITS (32)
#define L2WAY (8)
#define lengthof(arr) (sizeof(arr) / sizeof(arr[0]))
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
    address_t copy, tag, index, offset;
    copy = address;

    offset = copy & ((1 << bits_offset) - 1);
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
Cache cacheEvict(Cache** pcache, address_t const address, int const bits_index, int const bits_offset, int const associativity) {
    Cache c;
    Cache* pc = cacheFind(pcache, address, bits_index, bits_offset, associativity);
    if (pc == NULL) {
        printf("cacheEvict() function gets wrong arguments.\n");
        exit(1);
    }
    c = *pc;
    pc->dirty = CLEAN;
    pc->modified_time = -1;
    pc->tag = 0;
    pc->valid = INVALID;
    return c;
}

Cache cacheLoadnEvict(Cache** pcache, address_t const address, int const bits_index, int const bits_offset, int const associativity, int const readline, dirtybit_t isDirty) {
    Cache evictedCache;
    address_t copy, tag, index, offset;
    int LRU_set_index, LRU_time_max = INT_MAX;
    copy = address;

    offset = copy & ((1 << bits_offset) - 1);
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
    address_t addr_tag_L1, addr_index_L1, addr_offset_L1;
    address_t addr_tag_L2, addr_index_L2, addr_offset_L2;
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
    const int bits_tag_L1 = BITS - bits_index_L1 - bits_offset_L1;
    const int bits_tag_L2 = BITS - bits_index_L2 - bits_offset_L2;
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
        int isL1miss = 0, isL2miss = 0;
        int LRU_set_index_L1, LRU_set_index_L2;
        int LRU_time_L1 = INT_MAX, LRU_time_L2 = INT_MAX;
        Cache* findc = NULL;
        Cache evicted;
        address_t copy = address;

        addr_offset_L1 = copy & ((1 << bits_offset_L1) - 1);
        copy >>= bits_offset_L1;
        addr_index_L1 = copy & ((1 << bits_index_L1) - 1);
        copy >>= bits_index_L1;
        addr_tag_L1 = copy;
        copy = address;
        addr_offset_L2 = copy & ((1 << bits_offset_L2) - 1);
        copy >>= bits_offset_L2;
        addr_index_L2 = copy & ((1 << bits_index_L2) - 1);
        copy >>= bits_index_L2;
        addr_tag_L2 = copy;
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
                    if (!cacheFind(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity)) {
                        printf("Debuging level Fatal error occured.\n");
                    }
                    cacheFind(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity)->dirty = DIRTY;
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
            printf("Invalid command type.\n");
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
                    //작전: l2에서 가장 LRU적으로 문제있는 것을 찾는다.
                    //우선 그것을 제거한다.Dirty라면 메모리 1 늘린다.
                    //제거된것의 index는 넣은 것과 같다(Set이므로)
                    //아마도 offset을 임의로 설정해도 tag와 index만으로 L1 find가 가능
                    //L1find가 됬다면 그것도 저격해서 제거한다.
                    //Read이므로 dirty는 0이고 LoadnEvict를 각각 실행하여 주면 끝!
                    address_t backtrack;
                    evicted = cacheLoadnEvict(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity, readline, CLEAN);
                    L2_miss++;
                    if (evicted.dirty == DIRTY) {
                        L2_mwrite++;
                    }
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
                    cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, CLEAN);
                }
                else {
                    //L1 read miss but L2 hit
                    findc->modified_time = readline;
                    cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, findc->dirty);
                }
                break;
            case WRITE:
                L2_all++;
                if ((findc = cacheFind(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity)) == NULL) {
                    //L1 Write miss and L2 miss
                    address_t backtrack;
                    evicted = cacheLoadnEvict(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity, readline, DIRTY);
                    L2_miss++;
                    if (evicted.dirty == DIRTY) {
                        L2_mwrite++;
                    }
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
                    cacheLoadnEvict(cache_L1_D, address, bits_index_L1, bits_offset_L1, L1associativity, readline, DIRTY);
                }
                else {
                    //L1 read miss but L2 hit
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
                    evicted = cacheLoadnEvict(cache_L2, address, bits_index_L2, bits_offset_L2, L2associativity, readline, CLEAN);
                    L2_miss++;
                    if (evicted.dirty == DIRTY) {
                        L2_mwrite++;
                    }
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
    /*printf("Please Wait -");
    fflush(stdout);
    int progress = 0;*/
    for (int i = 0; i < lengthof(blocksize); i++) {
        for (int j = 0; j < lengthof(L1associativities); j++) {
            for (int k = 0; k < lengthof(L1size); k++) {
                result_in[i][j][k] = startSimulator(trace, L1size[k], L2size[0], blocksize[i], L1associativities[j], INCLUSIVE);
                /*switch (progress % 3) {
                case 0: printf("\b\\"); fflush(stdout); break;
                case 1: printf("\b/"); fflush(stdout); break;
                case 2: printf("\b-"); fflush(stdout); break;
                }
                progress++;*/
            }
        }
    }
    putchar('\r');
    for (int i = 0; i < sizeof("Please Wait -") - 1; i++) {
        putchar(' ');
    }
    putchar('\r');
    for (int i = 0; i < lengthof(blocksize); i++) {
        for (int j = 0; j < lengthof(L1associativities); j++) {
            for (int k = 0; k < lengthof(L1size); k++) {
                // result_ex[i][j][k] = startSimulator(trace, L1size[k], L2size[0], blocksize[i], L1associativities[j], EXCLUSIVE);
            }
        }
    }
    for (int j = 0; j < lengthof(L1associativities); j++) {
        for (int k = 0; k < lengthof(L1size); k++) {
            printf("%.2f%% ", result_in[0][j][k].missratio_I);
        }
        printf("\n");
    }
    fclose(trace);
}