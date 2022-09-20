#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int c(const void* _A, const void* _B) {
	if (*(int*)_A > *(int*)_B) return 1;
	else if (*(int*)_A < *(int*)_B) return -1;
	else return 0;
}
int getAnswer(int* arr, const int arrSize, const int limit) {
	qsort(arr, arrSize, sizeof(int), c);
	int lower = 0, upper = arr[arrSize - 1];
	int answer = 10000;
	while (lower <= upper) {
		int mid = lower + (upper - lower) / 2;
		int sum = 0;
		for (int i = 0; i < arrSize; i++) {
			if (arr[i] <= mid) sum += arr[i];
			else sum += mid;
		}
		if (sum <= limit) {
			lower = mid + 1;
			answer = mid;
		}
		else {
			upper = mid - 1;
		}
	}
	return answer;
}
int main() {
	int* arr;
	int N, limit;
	scanf("%d", &N);
	arr = calloc(N, sizeof(int));
	for (int i = 0; i < N; i++) {
		scanf("%d", &arr[i]);
	}
	scanf("%d", &limit);
	printf("%d\n", getAnswer(arr, N, limit));
	free(arr);
}