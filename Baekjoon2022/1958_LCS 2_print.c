#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMOFSTR (2)
#define MAXLEN (1002)
#define MAX(x, y) (x >= y ? x : y)
char* longestCommonSubsequence(const char** str_tokens, int* psize) {
	static int matrix[MAXLEN + 1][MAXLEN + 1];
	const int len1 = strlen(str_tokens[0]);
	const int len2 = strlen(str_tokens[1]);
	char* lcs;
	memset(matrix, 0, sizeof(matrix));
	for (int i = 0; i < len1; i++) {
		for (int j = 0; j < len2; j++) {
			if (str_tokens[0][i] == str_tokens[1][j]) {
				matrix[i + 1][j + 1] = matrix[i][j] + 1;
			}
			else {
				matrix[i + 1][j + 1] = MAX(matrix[i][j + 1], matrix[i + 1][j]);
			}

		}
	}
	const int lcslen = matrix[len1][len2];
	if (psize != NULL) *psize = lcslen;
	if ((lcs = calloc(lcslen + 1, sizeof(char))) < 0) exit(1);
	int rowmax = len1 - 1, colmax = len2 - 1;
	for (int i = lcslen - 1; i >= 0; i--) {
		for (int row = rowmax; row >= 0; row--) {
			for (int col = colmax; col >= 0; col--) {
				if (((i + 1) == matrix[row + 1][col + 1]) && str_tokens[0][row] == str_tokens[1][col]) {
					lcs[i] = str_tokens[0][row];
					rowmax = row - 1, colmax = col - 1;
					goto BREAK;
				}
			}
		} BREAK:;
	}
	return lcs;
}
int main() {
	char str_buffer[NUMOFSTR][MAXLEN];
	const char* str_tokens[NUMOFSTR];
	char* lcs;
	int lcs_len;
	for (int i = 0; i < NUMOFSTR; i++) {
		str_tokens[i] = str_buffer[i];
		scanf("%s", str_buffer[i]);
	}
	lcs = longestCommonSubsequence(str_tokens, &lcs_len);
	printf("%d\n", lcs_len);
	printf("%s\n", lcs);
}