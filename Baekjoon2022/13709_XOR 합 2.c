#include <stdio.h>
#include <stdlib.h>
#define MAXBIT (64)
long long xorMaximization(long long arr[], const int N) {
	long long max = 0;
	int pivot_num = 0;
	for (int i = MAXBIT - 2; i >= 0; i--) {
		for (int j = pivot_num; j < N; j++) {
			if ((arr[j] >> i & 1) == 0) continue;
			if (j != pivot_num) {
				long long temp = arr[j];
				arr[j] = arr[pivot_num];
				arr[pivot_num] = temp;
			}
			pivot_num++;
			for (int k = 0; k < N; k++) {
				if (k == pivot_num - 1) continue;
				if ((arr[k] >> i & 1) == 1) {
					arr[k] ^= arr[pivot_num - 1];
				}
			}
			break;
		}
	}
	for (int i = 0; i < pivot_num; i++) { max ^= arr[i]; }
	return max;
}
int main() {
	int N;
	long long* arr;
	scanf("%d", &N);
	if (!(arr = calloc(N, sizeof(long long)))) exit(1);
	for (int i = 0; i < N; i++) { scanf("%lld", &arr[i]); }
	printf("%lld\n", xorMaximization(arr, N));
	free(arr);
}