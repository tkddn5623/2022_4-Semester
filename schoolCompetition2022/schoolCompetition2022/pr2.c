#include <stdio.h>
int main() {
	double p[10];
	double min = 2;
	double temp, ret = 1;
	int minIdx;
	for (int i = 0; i < 10; i++) {
		scanf("%lf", &p[i]);
		if (min > p[i]) {
			min = p[i];
			minIdx = i;
		}
	}
	temp = p[minIdx];
	p[minIdx] = p[0];
	p[0] = temp;
	
	for (int i = 1; i < 10; i++) {
		ret = ret * (p[i] / i) * 10;
	}
	printf("%f\n", ret);
}