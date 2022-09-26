#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	int weight;
	int value;
} Item;
/*int c(const void* _A, const void* _B) {
	if (((Item*)(_A))->weight > ((Item*)(_B))->weight) return 1;
	else if (((Item*)(_A))->weight < ((Item*)(_B))->weight) return -1;
	else return 0;
}*/
int solveKnapsack(Item* items, const int N, const int maxweight) {
	static int firstCalled = 1;
	static int bests[100][100001] = { 0 };
	Item item;
	if (!firstCalled) memset(bests, 0, sizeof(bests));
	else firstCalled = 0;
	//qsort(items, N, sizeof(Item), c);
	item = items[0];
	for (int i = item.weight; i <= maxweight; i++) {
		bests[0][i] = item.value;
	}
	for (int i = 1; i < N; i++) {
		item = items[i];
		for (int j = 1; j < item.weight; j++) {
			bests[i][j] = bests[i - 1][j];
		}
		for (int j = item.weight; j <= maxweight; j++) {
			int a, b;
			a = bests[i - 1][j - item.weight] + item.value;
			b = bests[i - 1][j];
			bests[i][j] = a >= b ? a : b;
		}
	}
	/*{
		printf("---\n");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j <= maxweight; j++) {
				printf("%d, ", bests[i][j]);
			}
			printf("\n");
		}
		printf("---\n");
	}*/
	return bests[N - 1][maxweight];
}
int main() {
	{
		freopen("input.txt", "r", stdin);
	}
	int N, K;
	scanf("%d%d", &N, &K);
	Item* items = calloc(N, sizeof(Item));
	for (int i = 0; i < N; i++) {
		scanf("%d%d", &(items[i].weight), &(items[i].value));
	}
	printf("%d\n", solveKnapsack(items, N, K));
	free(items);
}