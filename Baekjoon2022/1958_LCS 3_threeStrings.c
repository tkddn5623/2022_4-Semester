#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMOFSTR (3)
#define MAXLEN (102)
#define MAX(x, y) (x >= y ? x : y)
#define MAX3(x, y, z) (MAX(x, MAX(y, z)))
int longestCommonSubsequence(const char** str) {
	static int matrix[MAXLEN + 1][MAXLEN + 1][MAXLEN + 1];
	const int len1 = strlen(str[0]);
	const int len2 = strlen(str[1]);
	const int len3 = strlen(str[2]);
	memset(matrix, 0, sizeof(matrix));
	for (int i = 0; i < len1; i++) {
		for (int j = 0; j < len2; j++) {
			for (int k = 0; k < len3; k++) {
				if (str[0][i] == str[1][j] && str[0][i] == str[2][k]) {
					matrix[i + 1][j + 1][k + 1] = matrix[i][j][k] + 1;
				}
				else {
					matrix[i + 1][j + 1][k + 1] = MAX3(matrix[i][j + 1][k + 1], matrix[i + 1][j][k + 1], matrix[i + 1][j + 1][k]);
				}
			}
		}
	}
	return matrix[len1][len2][len3];
}
int main() {
	char str[NUMOFSTR][MAXLEN];
	const char* str_token[NUMOFSTR];
	for (int i = 0; i < NUMOFSTR; i++) {
		str_token[i] = str[i];
		scanf("%s", str[i]);
	}
	printf("%d\n", longestCommonSubsequence(str_token));
}