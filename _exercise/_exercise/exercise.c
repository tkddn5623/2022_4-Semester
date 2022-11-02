#include <stdio.h>
int main() {
	{
		//freopen("i.txt", "r", stdin);
	}
	char C[100];
	int N, X, drink = 0;
	scanf("%d%d", &N, &X);
	for (int i = 0; i < N; i++) {
		scanf(" %c", &C[i]);
	}
	for (int i = 0; i < N; i++) {
		printf("%c %d ", C[i], i);
		if ((C[i] - X - 1) / N < (C[drink] - X - 1) / N) {
			drink = i;
		}
	}
	printf("%d", drink + 1);
}