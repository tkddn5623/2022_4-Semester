#include <stdio.h>
#define MAXBIT (60)
long long make_xor_basis(long long arr[], const int N) {
	int pivot_num = 0;
	for (int i = MAXBIT; i >= 0; i--) {
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
	return pivot_num;
}
long long minimum_xor_difference(long long arr[], const int N){
	long long xorall = 0;
	long long xorb = 0;
	int pivot_num;
	for (int i = 0; i < N; i++) { xorall ^= arr[i]; }
    for (int i = 0; i < N; i++) { arr[i] &= xorall; }
	pivot_num = make_xor_basis(arr, N);
	for (int i = 1; i < pivot_num; i++) { xorb ^= arr[i] & xorall; }
	return xorall - 2 * xorb;
}
int main() {
	int _T;
	int N;
	static long long arr[100000];
	scanf("%d", &_T);
	while (_T--) {
		scanf("%d", &N);
		for (int i = 0; i < N; i++) { scanf("%lld", &arr[i]); }
		printf("%lld\n", minimum_xor_difference(arr, N));
	}
}