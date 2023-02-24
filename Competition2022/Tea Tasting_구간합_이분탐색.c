/*
* Educational Codeforces Round 143 (Rated for Div. 2)
* Problem C
* Time limit exceeded on test 5
*/
#define __USE_MINGW_ANSI_STDIO 0
#include <stdio.h>
#include <string.h>
#define MAXSIZE (200000)
int binary_lower_geq(const long long arr[], int size, const long long key) {
    int mid, pos = size, left = 0;
    size = size - 1;
    while (left <= size) {
        mid = left + (size - left) / 2;
        if (arr[mid] >= key) {
            pos = mid;
            size = mid - 1;
        }
        else if (arr[mid] < key) left = mid + 1;
    }
    return pos;
}
void solve_1795_c(const int tea_amount[], const int test_limit[], int N) {
    static int change[MAXSIZE + 1];
    static long long remainder[MAXSIZE + 1];
    static long long prefixsum[MAXSIZE + 1];
    memset(change, 0, sizeof(int) * (N + 1));
    memset(remainder, 0, sizeof(long long) * (N + 1));
    prefixsum[0] = 0;
    for (int i = 0; i < N; i++) {
        prefixsum[i + 1] = prefixsum[i] + test_limit[i];
    }
    for (int i = 0; i < N; i++) {
        int j = binary_lower_geq(&prefixsum[i + 1], N - i, tea_amount[i] + prefixsum[i]) + i;
        change[i]++;
        change[j]--;
        remainder[j] += tea_amount[i] - prefixsum[j] + prefixsum[i];
    }
    for (int i = 0, j = 0; i < N; i++) {
        j += change[i];
        printf("%lld ", (long long)j * test_limit[i] + remainder[i]);
    }
    putchar('\n');
}
int main() {
    //{freopen("i.txt", "r", stdin); }
    static int tea_amount[MAXSIZE];
    static int test_limit[MAXSIZE];
    int _t, N;
    scanf("%d", &_t);
    while (_t--) {
        scanf("%d", &N);
        for (int i = 0; i < N; i++) { scanf("%d", &tea_amount[i]); }
        for (int i = 0; i < N; i++) { scanf("%d", &test_limit[i]); }
        solve_1795_c(tea_amount, test_limit, N);
    }
}
