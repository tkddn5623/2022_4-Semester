#include <stdlib.h>
#include <string.h>
#include "LinearSieve.h"

int* getPrimeList(const int maxprime, int* psize) {
	int length = 0;
	int* notPrime = calloc(maxprime + 1, sizeof(int));
	int* primeList = calloc(maxprime, sizeof(int));
	int* shrunkList;
	if (notPrime == NULL || primeList == NULL) exit(1);
	notPrime[0] = 1;
	notPrime[1 <= maxprime ? 1 : 0] = 1;
	for (int i = 2; i <= maxprime; i++) {
		if (!notPrime[i]) primeList[length++] = i;
		for (int j = 0; j < length; j++) {
			if (i * primeList[j] > maxprime) break;
			notPrime[i * primeList[j]] = 1;
			if (i % primeList[j] == 0) break;
		}
	}
	free(notPrime);
	if (((shrunkList = malloc(length * sizeof(int))) == NULL)) exit(1);
	memcpy(shrunkList, primeList, length * sizeof(int));
	free(primeList);
	if (psize != NULL) *psize = length;
	return shrunkList;
}

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