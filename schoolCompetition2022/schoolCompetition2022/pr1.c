#include <stdio.h>
#include <stdlib.h>
int main() {
	int* T;
	int j, N, X;
	int esc = 0;
	scanf("%d%d", &N, &X);
	T = calloc(N + 1, sizeof(int));
	for (int i = 1; i <= N; i++) {
		scanf("%d", &T[i]);
	}
	while (!esc) {
		for (j = 1; j <= N; j++) {
			if (X > T[j]) {
				esc = 1;
				break;
			}
			else X++;
		}
	}
	printf("%d\n", j);
}