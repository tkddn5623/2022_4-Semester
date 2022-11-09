#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void* thread(void* _arg) {
    int** const argl = (int**)_arg;
    const int row = (int)argl[3];
    const int colsize = (int)argl[4];
    for (int i = 0; i < colsize; i++) {
        argl[2][row] += argl[0][i] * argl[1][i];
    }
    free(argl);
    pthread_exit(NULL);
}
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("You should input row and col.\n");
        exit(1);
    }
    const int row = atoi(argv[1]);
    const int col = atoi(argv[2]);
    pthread_t* tid;
    int** mat;
    int* v1, * v2;
    /* Initialization step. */
    srand(time(NULL));
    if ((tid = calloc(row, sizeof(pthread_t))) == NULL) exit(1);
    if ((mat = calloc(row, sizeof(int*))) == NULL) exit(1);
    if ((v1 = calloc(col, sizeof(int))) == NULL) exit(1);
    if ((v2 = calloc(row, sizeof(int))) == NULL) exit(1);
    for (int i = 0; i < row; i++) {
        if ((mat[i] = calloc(col, sizeof(int))) == NULL) exit(1);
        for (int j = 0; j < col; j++) {
            mat[i][j] = rand() % 10;
        }
    }
    for (int i = 0; i < col; i++) {
        v1[i] = rand() % 10;
    }
    /* Thread making step. */
    for (int i = 0; i < row; i++) {
        int** arglist;
        if ((arglist = calloc(5, sizeof(int*))) == NULL) exit(1);
        arglist[0] = mat[i];
        arglist[1] = v1;
        arglist[2] = v2;
        arglist[3] = i;
        arglist[4] = col;
        if (pthread_create(&tid[i], NULL, thread, (void*)arglist)) exit(1);
    }
    for (int i = 0; i < row; i++) {
        pthread_join(tid[i], NULL);
    }
    /* Output step. */
    printf(" *** Matirx ***\n");
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("[ %d ] ", mat[i][j]);
        }
        printf("\n");
    }
    printf(" *** Vector ***\n");
    for (int i = 0; i < col; i++) {
        printf("[ %d ] \n", v1[i]);
    }
    printf(" *** Result ***\n");
    for (int i = 0; i < row; i++) {
        printf("[ %d ] \n", v2[i]);
    }

    
    free(tid);
    free(v1);
    free(v2);
    for (int i = 0; i < row; i++) {
        free(mat[i]);
    }
    free(mat);
}