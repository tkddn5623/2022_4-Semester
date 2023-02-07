//solve1786_RabinKarp, 2023_2_7 Tue.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN (1000000)
#define POLYCOEF (2)
#define HASHSIZE (1 << 20) //Unless it is power of 2, This code makes errors, At line 33~34.
unsigned int polyhash(const char str[], int N, unsigned int* retpower) {
	unsigned int h = 0;    //(If HASHSIZE is not power of 2) Please consider change to long long, or place modular at line 12.
	unsigned int power = 1;
	for (char* p = &str[N - 1]; p >= str; p--) {
		h += (*p) * power;
		power = (power * POLYCOEF) % HASHSIZE;
	}
	if (retpower != NULL) *retpower = power;
	return h % HASHSIZE;
}
int* solve1786_RabinKarp(const char text[], const char pattern[]) {
	const int len_text = strlen(text);
	const int len_ptrn = strlen(pattern);
	int* answerlist = calloc(len_text - len_ptrn + 2, sizeof(int));
	if (!answerlist) exit(1);
	if (len_text < len_ptrn) return answerlist;
	int count = 0;
	unsigned int power;
	unsigned int hash_text = polyhash(text, len_ptrn, &power);
	const unsigned int hash_ptrn = polyhash(pattern, len_ptrn, NULL);

	if (hash_text == hash_ptrn && !strncmp(text, pattern, len_ptrn)) {
		answerlist[++count] = 1;
	}
	for (int i = 1; i <= len_text - len_ptrn; i++) {
		hash_text = hash_text * POLYCOEF + text[i + len_ptrn - 1] - text[i - 1] * power;
		hash_text %= HASHSIZE;
		if (hash_text == hash_ptrn && !strncmp(text + i, pattern, len_ptrn)) {
			answerlist[++count] = i + 1;
		}
	}
	answerlist[0] = count;
	return answerlist;
}
int main() {
	char* cp;
	int* answerlist;
	static char text[MAXLEN + 2];
	static char pattern[MAXLEN + 2];

	fgets(text, sizeof(text), stdin);
	fgets(pattern, sizeof(pattern), stdin);
	if (cp = strchr(text, '\n')) *cp = '\0';
	if (cp = strchr(pattern, '\n')) *cp = '\0';

	answerlist = solve1786_RabinKarp(text, pattern);
	for (int N = answerlist[0], i = 0; i <= N; i++) {
		printf("%d\n", answerlist[i]);
	}
	free(answerlist);
}