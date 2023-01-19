#include <stdio.h>
#include <stdlib.h>
#define MAXDIM (500)
double** inverseMatrix(double mat[][MAXDIM], const int N) {
	double** inverse;
	int pivot_num = 0;
	if (!(inverse = calloc(N, sizeof(double*)))) exit(1);
	for (int i = 0; i < N; i++) { if (!(inverse[i] = calloc(N, sizeof(double)))) exit(1); }
	for (int i = 0; i < N; i++) { inverse[i][i] = 1; }
	for (int c = 0; c < N; c++) {
		for (int r = pivot_num; r < N; r++) {
			if (mat[r][c] == 0) continue;
			double lead = mat[r][c];
			if (lead != 1) {
				for (int i = 0; i < N; i++) { inverse[r][i] /= lead; }
				for (int i = c + 1; i < N; i++) { mat[r][i] /= lead; } //Make leading entry 1
				mat[r][c] = 1;
			}
			if (r != pivot_num) { //Row interchange
				for (int i = 0; i < N; i++) {
					double temp = mat[r][i];
					mat[r][i] = mat[pivot_num][i];
					mat[pivot_num][i] = temp;

					temp = inverse[r][i];
					inverse[r][i] = inverse[pivot_num][i];
					inverse[pivot_num][i] = temp;
				}
			}
			pivot_num++;
			break;
		}
		if (c != pivot_num - 1) { printf("no inverse\n"); exit(0); }
		for (int r = 0; r < N; r++) {
			if (r == pivot_num - 1) continue;
			if (mat[r][c] != 0) {
				double weight = mat[r][c];
				for (int i = 0; i < N; i++) {
					inverse[r][i] -= weight * inverse[pivot_num - 1][i];
				}
				for (int i = c + 1; i < N; i++) {
					mat[r][i] -= weight * mat[pivot_num - 1][i];
				}
				mat[r][c] = 0;
			}
		}
	}
	return inverse;
}
int main() {
	static double mat[MAXDIM][MAXDIM];
	double** inverse;
	int N;
	scanf("%d", &N);
	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) { scanf("%lf", &mat[i][j]); }
	inverse = inverseMatrix(mat, N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) { printf("%f ", inverse[i][j]); }
		putchar('\n');
	}
	for (int i = 0; i < N; i++) { free(inverse[i]); }
	free(inverse);
}