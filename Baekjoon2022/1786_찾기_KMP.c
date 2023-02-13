#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN (1000000)
int* solve1786_KMP(const char text[], const char pattern[]) {
	static int prefix[MAXLEN];
	const int len_text = strlen(text);
	const int len_ptrn = strlen(pattern);
	int* answerlist = calloc(len_text - len_ptrn + 2, sizeof(int));
	int answersize = 0;
	if (!answerlist) exit(1);
	if (len_text < len_ptrn) return answerlist;

	// Compute-Prefix-Function
	prefix[0] = 0;
	for (int i = 1, k = 0; i < len_ptrn; i++) {
		while (k > 0 && pattern[k] != pattern[i])
			k = prefix[k - 1];
		if (pattern[k] == pattern[i])
			k = k + 1;
		prefix[i] = k;
	}

	// KMP-Matcher
	for (int i = 0, q = 0; i < len_text; i++) {
		while (q > 0 && text[i] != pattern[q])
			q = prefix[q - 1];
		if (text[i] == pattern[q])
			q = q + 1;
		if (q == len_ptrn) {
			q = prefix[q - 1];
			answerlist[++answersize] = i - len_ptrn + 2;
			//answerlist[++answersize] = i - len_ptrn + 1;
		}
	}
	answerlist[0] = answersize;
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

	answerlist = solve1786_KMP(text, pattern);
	for (int N = answerlist[0], i = 0; i <= N; i++) {
		printf("%d\n", answerlist[i]);
	}
	free(answerlist);
}