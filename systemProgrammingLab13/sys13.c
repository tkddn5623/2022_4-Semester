
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#define RANDSEED (0) // 0 : random seed will be time(NULL)
#define RADIUS (3640)

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int point_in_circle = 0;

void* pt_monte(void* arg) {
    const double RADIUS_sq = (long long)RADIUS * RADIUS;
    const int it = *((int*)arg);
    double dist_sq;
    int x, y;
    for (int i = 0; i < it; i++) {
        x = rand() % (RADIUS + 1);
        y = rand() % (RADIUS + 1);
        dist_sq = ((long long)x * x) + ((long long)y * y);
        if (dist_sq <= RADIUS_sq) {
            pthread_mutex_lock(&lock);
            point_in_circle++;
            printf("point_in_circle : %d\n", point_in_circle);
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(NULL);
}
int main(int argc, char* argv[]) {
    pthread_t* tid;
    int num_thread, point_per_thread;
    long long totalPoint;
    double pi;
    if (argc <= 2 || !(num_thread = atoi(argv[1])) || !(point_per_thread = atoi(argv[2]))) {
        printf("Please input correct arguments.\n(1) the number of threads (2) the number of random points per thread\n");
        exit(1);
    }
    if (RADIUS < 1) {
        printf("Please set correct radius macro. It shoud be integer greater than 0.\n");
        exit(1);
    }
    RANDSEED == 0 ? srand(time(NULL)) : srand(RANDSEED);
    tid = calloc(num_thread, sizeof(pthread_t));
    totalPoint = num_thread * point_per_thread;
    for (int i = 0; i < num_thread; i++) {
        if (pthread_create(&tid[i], NULL, pt_monte, &point_per_thread)) exit(1);
    }
    for (int i = 0; i < num_thread; i++) {
        pthread_join(tid[i], NULL);
    }
    pi = (double)(4 * point_in_circle) / totalPoint;
    printf("PI was approximated as [%f]\n", pi);
    free(tid);
}