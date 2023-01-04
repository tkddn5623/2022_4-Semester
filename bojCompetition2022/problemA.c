#include <stdio.h>
#include <stdlib.h>
#define MAX (5000)
int* getFactorList(const int maxprime) {
	int length = 0;
	int* factorList = calloc(maxprime + 2, sizeof(int));
	int* primeList = calloc(maxprime, sizeof(int));
	if (factorList == NULL || primeList == NULL) exit(1);
	factorList[0] = -1, factorList[1] = -1;
	for (int i = 2; i <= maxprime; i++) {
		if (!factorList[i]) primeList[length++] = i;
		for (int j = 0; j < length; j++) {
			if (i * primeList[j] > maxprime) break;
			factorList[i * primeList[j]] = primeList[j];
			if (i % primeList[j] == 0) break;
		}
	}
	free(primeList);
	return factorList;
}
long long int sumFactor(const int X, const int* factor) {
	static int isInit = 0;
	static long long int* memo = NULL;
	if (!isInit) {
		isInit = 1;
		memo = calloc(MAX + 1, sizeof(long long int));
		memo[1] = 0;
		long long int answer = 0;
		for (int i = 2; i <= MAX; i++) {
			int number = i, base = 1, basesquare = 1, subproduct = 1;
			int product = 1;
			answer = 0;
			while (1) {
				if (factor[number] == 0) {
					if (number == base) {
						basesquare *= number;
						subproduct += basesquare;
						product *= subproduct;
						break;
					}
					else {
						product = product * subproduct * (1 + number);
						break;
					}
				}
				else if (factor[number] != base) {
					product *= subproduct;
					base = factor[number];
					basesquare = 1;
					subproduct = 1;
					continue;
				}
				else {
					basesquare *= factor[number];
					subproduct += basesquare;
					number /= factor[number];
				}
			}
			answer += product;
			memo[i] = answer - (i);
		}
	}
	return memo[X];
}
int main() {
	int* factor = getFactorList(MAX);
	int* blacklist = calloc(MAX + 1, sizeof(int));
	int TC;
	for (int i = 1; i <= MAX; i++) {
		if (sumFactor(i, factor) > i) {
			for (int j = 2; j <= MAX; j++) {
				if (i * j > MAX) break;
				blacklist[i * j] = 1;
			}
		}
	}
	scanf("%d", &TC);
	for (int _TC = 0; _TC < TC; _TC++) {
		int n;
		scanf("%d", &n);
		if (!blacklist[n] && sumFactor(n, factor) > n) printf("Good Bye\n");
		else printf("BOJ 2022\n");
	}
	free(factor);
	free(blacklist);
}