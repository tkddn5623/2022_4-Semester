#include <stdio.h>
#define MAXN (128)
int* solve2630(int mat[][MAXN + 1], int row, int col, int N) {
	static int count[2] = { 0 };
	const int color = mat[row][col];
	int no_more_call = 1;
	for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
		if (mat[i + row][j + col] != color) {
			no_more_call = 0;
			solve2630(mat, row, col, N / 2);
			solve2630(mat, row, col + N / 2, N / 2);
			solve2630(mat, row + N / 2, col, N / 2);
			solve2630(mat, row + N / 2, col + N / 2, N / 2);
			goto ESC;
		}
	} ESC:;
	if (no_more_call) count[color]++;
	return count;
}
int main() {
	static int mat[MAXN + 1][MAXN + 1] = { 0 };
	int N;
	int* answer;
	scanf("%d", &N);
	if (N <= 0 || N & 1) { return 1; }
	for (int i = 1; i <= N; i++) for (int j = 1; j <= N; j++) {
		scanf("%d", &mat[i][j]);
	}
	answer = solve2630(mat, 1, 1, N);
	printf("%d\n%d\n", answer[0], answer[1]);
}