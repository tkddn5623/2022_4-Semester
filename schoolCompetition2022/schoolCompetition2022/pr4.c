#include <stdio.h>
int bsch(int left, int right) {
	while (left <= right) {
		const int key = 0;
		int mid, sheep, diff;
		mid = (left + right) / 2;
		printf("? %d\n", mid);
		fflush(stdout);
		scanf("%d", &sheep);
		diff = (2 * sheep) - mid;
		if (diff == 0) return mid;
		else if (diff < 0) right = mid - 1;
		else left = mid + 1;
	}
	return 0;
}
int main() {
	int N, ret;
	scanf("%d", &N);
	ret = bsch(1, N);
	if (ret == 0) printf("! 0\n");
	else printf("! %d\n", ret);
}