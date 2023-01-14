#include <stdio.h>
int multiply(int A, int B, int C) {
	if (B == 1) return A % C;
	int cur = multiply(A, B / 2, C);
	if (B % 2 == 1) return (((long long)cur * cur) % C * A) % C;
	else return ((long long)cur * cur) % C;
}
int main() {
	int A, B, C;
	scanf("%d%d%d", &A, &B, &C);
	printf("%d\n", multiply(A, B, C));
}