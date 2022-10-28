#include <stdio.h>
#include <stdlib.h>
#define MAX (10000000)
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
int main() {
	int* factors = getFactorList(MAX);
	int answer = 0;
	int S, T;
	scanf("%d%d", &S, &T);
	if (S == 1) {
		answer = -1;
		S = 2;
	}
	for (int i = S; i <= T; i++) {
		int number = i;
		int numOfno2factors = 1;
		int count = 0, count2 = 0;
		int prev = factors[number];
		while (1) {
			if (factors[number] == 0) {
				if (number == 2) count2++;
				else if (prev == number) numOfno2factors *= (count + 2);
				else numOfno2factors *= (count + 1) * 2;
				
				break;
			}
			else {
				if (factors[number] == 2) {
					count2++;
				}
				else if(prev == factors[number]) {
					count++;
				}
				else {
					numOfno2factors *= (count + 1);
					count = 1;
				}
				prev = factors[number];
				number /= factors[number];
			}
		}
		answer += (count2 - 1) * numOfno2factors;

	}
	printf("%d\n", answer);
	free(factors);
}