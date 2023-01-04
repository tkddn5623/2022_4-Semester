//https://www.acmicpc.net/source/32729072
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#define LIST_MAX 100001
#define VMAX 199998

typedef struct Node {
    int num;
    int dist;
    struct Node * next;
} Node;

Node vertices[VMAX], * adj_list[LIST_MAX];
int max;
char * rp;

int search(int, int);
int read_int(void);

int main(void) {
    int V, i, idx, num, dist;
    Node * node, *vp;
    char * input;
    vp = vertices;
    struct stat sb;
    fstat(0, &sb);
    input = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    rp = input;
    V = read_int();
    for (i = 0; i < V; i++) {
        idx = read_int();
        num = read_int();
        dist = read_int();
        for (;;) {
            node = vp++;
            node->num = num;
            node->dist = dist;
            node->next = adj_list[idx];
            adj_list[idx] = node;
            num = read_int();
            if (num == -1) break;
            dist = read_int();
        }
    }
    search(1, 0);
    printf("%d\n", max);
    fflush(stdout);
    _exit(0);
}

int read_int(void) {
    register int n = 0;
    char c;
    if (*rp == '-') {
        rp += 3;
        return -1;
    }
    while ((c = *rp++) >= '0') {
        n = (n << 3) + (n << 1) + (c & 0xF);
    }
    return n;
}

int search(int cur, int prev) {
    int dist, first, second, sum;
    first = second = 0;
    Node * node = adj_list[cur];
    while (node) {
        if (node->num != prev) {
            dist = node->dist + search(node->num, cur);
            if (dist > first) {
                second = first;
                first = dist;
            } else if (dist > second) {
                second = dist;
            }
        }
        node = node->next;
    }
    sum = first + second;
    max = sum > max ? sum : max;
    return first;
}