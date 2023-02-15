#include <stdlib.h>
#include <string.h>
#include "KMP_Matcher.h"
#ifndef MAXLEN
#define MAXLEN (1000000)
#endif
int* KMP_Matcher(const char text[], const char pattern[]) {
	static int prefix[MAXLEN];
	const int len_text = strlen(text);
	const int len_ptrn = strlen(pattern);
	int* answerlist = calloc(len_text - len_ptrn + 2, sizeof(int));
	int answersize = 0;
	if (!answerlist) exit(1);
	if (len_text < len_ptrn) return answerlist;

	// Compute-Prefix-Function
	prefix[1] = 0;
	for (int i = 1, k = 0; i < len_ptrn; i++) {
		while (k > 0 && pattern[k] != pattern[i])
			k = prefix[k];
		if (pattern[k] == pattern[i])
			k = k + 1;
		prefix[i + 1] = k;
	}

	// KMP-Matcher
	for (int i = 0, q = 0; i < len_text; i++) {
		while (q > 0 && text[i] != pattern[q])
			q = prefix[q];
		if (text[i] == pattern[q])
			q = q + 1;
		if (q == len_ptrn) {
			q = prefix[q];
			answerlist[++answersize] = i - len_ptrn + 2;
			//answerlist[++answersize] = i - len_ptrn + 1;
		}
	}
	answerlist[0] = answersize;
	return answerlist;
}