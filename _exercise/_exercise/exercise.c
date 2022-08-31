#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int getAnswer(int* stones, const int N) {
	
	return 0;
}
int main() {
	int N;
	int* stones;
	scanf("%d", &N);
	if ((stones = calloc(N, sizeof(int))) == NULL) exit(1);
	for (int i = 0; i < N; i++) {
		scanf("%d", &stones[i]);
	}
	getAnswer(stones, N);
}