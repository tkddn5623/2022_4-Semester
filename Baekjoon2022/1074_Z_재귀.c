#include <stdio.h>
int solve1074(int N, int row, int col) {
	int count = 0;
	while (N >= 1) {
		int mid = ((1 << N) - 1) >> 1;
		int weight = 1 << ((N << 1) - 2);
		if (row > mid) {
			count += (weight << 1);
			row -= (mid + 1);
		}
		if (col > mid) {
			count += weight;
			col -= (mid + 1);
		}
		N--;
	}
	return count;
}
int main() {
	int N, r, c;
	scanf("%d%d%d", &N, &r, &c);
	printf("%d\n", solve1074(N, r, c));
}