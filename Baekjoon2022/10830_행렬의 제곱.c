#include <stdio.h>
#define DIM (5)
#define DIVIDER (1000)
int* matmul(int dest[][DIM], int operand[][DIM], int N) {
	int mat[DIM][DIM] = { 0 };
	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
		int sum = 0;
		for (int k = 0; k < N; k++) { sum += (operand[i][k] * dest[k][j]) % DIVIDER; }
		mat[i][j] = sum % DIVIDER;
	}
	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
		dest[i][j] = mat[i][j];
	}
	return dest;
}
int* matsqr(int dest[][DIM], int N, long long E) {
	int mat[DIM][DIM];
	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) { mat[i][j] = dest[i][j]; }
	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) { dest[i][j] = 0; }
	for (int i = 0; i < N; i++) { dest[i][i] = 1; }
	while (E > 0LL) {
		if (E % 2 == 1) { matmul(dest, mat, N); }
		matmul(mat, mat, N);
		E /= 2;
	}
	return dest;
}
int main() {
	int mat[DIM][DIM];
	int N;
	long long B;
	scanf("%d%lld", &N, &B);
	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) { 
		scanf("%d", &mat[i][j]); 
	}
	matsqr(mat, N, B);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) { printf("%d ", mat[i][j]); }
		putchar('\n');
	}
}