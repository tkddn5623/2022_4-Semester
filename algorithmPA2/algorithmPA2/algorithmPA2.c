#include <stdio.h>
#define MAXSTR (5)
#define MAXALIGN (20000)
#define MAXLEN (123)
#define MAX(x, y) (x >= y ? x : y)
#define MAX3(x, y, z) (MAX(x, MAX(y, z)))
#define EQUAL3(x, y, z) ((x == y) && (y == z))


int mystrlen(const char* _Str);
char* mystrcpy(char* _Dest, const char* _Src);
int mystrchr_id(const char* _Str, const char ch);
char* mystrcat(char* _Dest, const char* _Src);
char* mystrcat_ch(char* _Dest, const char ch);
int LCS_len(const char* str1, const char* str2);
char* LCS_make(char* dest, const char* str1, const char* str2);
int LCS_3_len(const char* str1, const char* str2, const char* str3);
char* LCS_3_make(char* dest, const char* str1, const char* str2, const char* str3);

int main() {
	FILE* fp_i, * fp_o;
	int K;
	char str_buffer[MAXSTR][MAXLEN] = { 0 };
	const char* str_tokens[MAXSTR] = { 0 };
	char str_aligned[MAXSTR + 1][MAXALIGN] = { 0 };
	char lcs[MAXLEN] = { 0 };
	int lcs_len = 0;
	char temp[MAXLEN] = { 0 };

	fp_i = fopen("hw2_input.txt", "r");
	fp_o = fopen("hw2_output.txt", "w");
	if (fp_i == NULL || fp_o == NULL) return 1;

	fgets(str_buffer[0], sizeof(str_buffer[0]), fp_i);
	sscanf(str_buffer[0], "%d", &K);
	fgets(str_buffer[0], sizeof(str_buffer[0]), fp_i);
	for (int i = 0; i < K; i++) {
		fgets(str_buffer[i], sizeof(str_buffer[i]), fp_i);
		int len = mystrlen(str_buffer[i]);
		for (int j = len - 1; j >= 0; j--) {
			if (str_buffer[i][j] == '\n') {
				str_buffer[i][j] = '\0';
			}
			if (str_buffer[i][j] == '\r') {
				str_buffer[i][j] = '\0';
			}
		}
		str_tokens[i] = str_buffer[i];
	}
	
	int visited[MAXSTR] = { 0 };
	int big_hei = 2, big_row = 1, big_col = 0, localmaxlen;
	localmaxlen = -1;
	if (K >= 3) {
		for (int i = 0; i < K; i++) {
			for (int j = 0; j < i; j++) {
				for (int k = 0; k < j; k++) {
					int len = LCS_3_len(str_tokens[i], str_tokens[j], str_tokens[k]);
					if (localmaxlen < len) {
						localmaxlen = len;
						big_hei = i;
						big_row = j;
						big_col = k;
					}
				}
			}
		}
		visited[big_hei] = 1;
		visited[big_row] = 1;
		visited[big_col] = 1;
		LCS_3_make(lcs, str_tokens[big_hei], str_tokens[big_row], str_tokens[big_col]);
	}
	else {
		for (int i = 0; i < K; i++) {
			for (int j = 0; j < i; j++) {
				int len = LCS_len(str_tokens[i], str_tokens[j]);
				if (localmaxlen < len) {
					localmaxlen = len;
					big_row = i;
					big_col = j;
				}
			}
		}
		visited[big_row] = 1;
		visited[big_col] = 1;
		LCS_make(lcs, str_tokens[big_row], str_tokens[big_col]);
	}
	
	if (K == 4) {
		for (int i = 0; i < K; i++) {
			if (visited[i]) continue;
			big_col = i;
		}
		visited[big_col] = 1;
		LCS_make(temp, lcs, str_tokens[big_col]);
		mystrcpy(lcs, temp);
	}
	else if (K == 5) {
		for (int i = 0; i < K; i++) {
			for (int j = 0; j < i; j++) {
				if (visited[i] || visited[j]) continue;
				big_row = i, big_col = j;
			}
		}
		visited[big_row] = 1;
		visited[big_col] = 1;
		LCS_3_make(temp, lcs, str_tokens[big_row], str_tokens[big_col]);
		mystrcpy(lcs, temp);
	}

	if ((lcs_len = mystrlen(lcs)) == 0) {
		for (int i = 0; i < K - 1; i++) {
			fprintf(fp_o, "%s\n", str_buffer[i]);
		}
		fprintf(fp_o, "%s", str_buffer[K - 1]);
	}
	else {
		int aligned_maxlen = -1;
		int aligned_len[MAXSTR] = { 0 };
		int left[MAXSTR] = { 0 };
		int right[MAXSTR] = { 0 };
		for (int i = 0; i < lcs_len; i++) {
			int right_left_max = -1;
			for (int j = 0; j < K; j++) {
				right[j] = mystrchr_id((str_tokens[j] + left[j]), lcs[i]) + left[j];
				if (right_left_max < (right[j] - left[j])) {
					right_left_max = right[j] - left[j];
				}
			}
			for (int j = 0; j < K; j++) {
				for(int k = 0; k < right_left_max; k++){
					if (left[j] < right[j]) {
						mystrcat_ch(str_aligned[j], str_tokens[j][left[j]]);
						left[j]++;
					}
					else {
						mystrcat_ch(str_aligned[j], '-');
					}
				}
				mystrcat_ch(str_aligned[j], lcs[i]);
				left[j] = right[j] + 1;
			}
			for (int k = 0; k < right_left_max; k++) {
				mystrcat_ch(str_aligned[K], ' ');
			}
			mystrcat_ch(str_aligned[K], '*');
		}
		for (int i = 0; i < K; i++) {
			mystrcat(str_aligned[i], str_tokens[i] + left[i]);
			aligned_len[i] = mystrlen(str_aligned[i]);
			if (aligned_maxlen < aligned_len[i]) aligned_maxlen = aligned_len[i];
		}
		for (int i = 0; i < K; i++) {
			int diff = aligned_maxlen - aligned_len[i];
			for (int j = 0; j < diff; j++) {
				mystrcat_ch(str_aligned[i], '-');
			}
		}
		for (int i = 0; i < K; i++) {
			fprintf(fp_o, "%s\n", str_aligned[i]);
		}
		fprintf(fp_o, "%s", str_aligned[K]);
	}
	fclose(fp_i);
	fclose(fp_o);
}
int mystrlen(const char* _Str) {
	int len = 0;
	while (_Str[len] != '\0')
		len++;
	return len;
}
char* mystrcpy(char* _Dest, const char* _Src) {
	int i;
	for (i = 0; _Src[i] != '\0'; i++) {
		_Dest[i] = _Src[i];
	}
	_Dest[i] = '\0';
	return _Dest;
}
int mystrchr_id(const char* _Str, const char ch) {
	int i = 0;
	while (_Str[i] != '\0') {
		if (_Str[i] == ch) return i;
		else i++;
	}
	return -1;
}
char* mystrcat(char* _Dest, const char* _Src) {
	int i = 0;
	while (_Dest[i] != '\0') {
		i++;
	}
	mystrcpy(_Dest + i, _Src);
	return _Dest;
}
char* mystrcat_ch(char* _Dest, const char ch) {
	int i = 0;
	while (_Dest[i] != '\0') {
		i++;
	}
	_Dest[i] = ch;
	_Dest[i + 1] = '\0';
	return _Dest;
}
int LCS_len(const char* str1, const char* str2) {
	int matrix[MAXLEN][MAXLEN] = { 0 };
	const int len1 = mystrlen(str1);
	const int len2 = mystrlen(str2);
	for (int i = 0; i < len1; i++) {
		for (int j = 0; j < len2; j++) {
			if (str1[i] == str2[j]) matrix[i + 1][j + 1] = matrix[i][j] + 1;
			else matrix[i + 1][j + 1] = MAX(matrix[i][j + 1], matrix[i + 1][j]);
		}
	}
	const int lcslen = matrix[len1][len2];
	return lcslen;
}
char* LCS_make(char* dest, const char* str1, const char* str2) {
	int matrix[MAXLEN][MAXLEN] = { 0 };
	const int len1 = mystrlen(str1);
	const int len2 = mystrlen(str2);
	for (int i = 0; i < len1; i++) {
		for (int j = 0; j < len2; j++) {
			if (str1[i] == str2[j]) matrix[i + 1][j + 1] = matrix[i][j] + 1;
			else matrix[i + 1][j + 1] = MAX(matrix[i][j + 1], matrix[i + 1][j]);
		}
	}
	const int lcslen = matrix[len1][len2];
	if (dest != NULL) {
		int rowmax = len1 - 1, colmax = len2 - 1;
		dest[lcslen] = '\0';
		for (int i = lcslen - 1; i >= 0; i--) {
			for (int row = rowmax; row >= 0; row--) {
				for (int col = colmax; col >= 0; col--) {
					if (((i + 1) == matrix[row + 1][col + 1]) && str1[row] == str2[col]) {
						dest[i] = str1[row];
						rowmax = row - 1, colmax = col - 1;
						goto BREAK;
					}
				}
			} BREAK:;
		}
	}
	return dest;
}
int LCS_3_len(const char* str1, const char* str2, const char* str3) {
	static int matrix[MAXLEN][MAXLEN][MAXLEN] = { 0 };
	const int len1 = mystrlen(str1);
	const int len2 = mystrlen(str2);
	const int len3 = mystrlen(str3);
	for (int i = 0; i <= len1; i++) {
		for (int j = 0; j <= len2; j++) {
			for (int k = 0; k <= len3; k++)
				matrix[i][j][k] = 0;
		}
	}
	for (int i = 0; i < len1; i++) {
		for (int j = 0; j < len2; j++) {
			for (int k = 0; k < len3; k++) {
				if (str1[i] == str2[j] && str2[j] == str3[k])
					matrix[i + 1][j + 1][k + 1] = matrix[i][j][k] + 1;
				else
					matrix[i + 1][j + 1][k + 1] = MAX3(matrix[i][j + 1][k + 1], matrix[i + 1][j][k + 1], matrix[i + 1][j + 1][k]);
			}
		}
	}
	const int lcslen = matrix[len1][len2][len3];
	return lcslen;
}
char* LCS_3_make(char* dest, const char* str1, const char* str2, const char* str3) {
	static int matrix[MAXLEN][MAXLEN][MAXLEN] = { 0 };
	const int len1 = mystrlen(str1);
	const int len2 = mystrlen(str2);
	const int len3 = mystrlen(str3);
	for (int i = 0; i <= len1; i++) {
		for (int j = 0; j <= len2; j++) {
			for (int k = 0; k <= len3; k++)
				matrix[i][j][k] = 0;
		}
	}
	for (int i = 0; i < len1; i++) {
		for (int j = 0; j < len2; j++) {
			for (int k = 0; k < len3; k++) {
				if (str1[i] == str2[j] && str2[j] == str3[k])
					matrix[i + 1][j + 1][k + 1] = matrix[i][j][k] + 1;
				else
					matrix[i + 1][j + 1][k + 1] = MAX3(matrix[i][j + 1][k + 1], matrix[i + 1][j][k + 1], matrix[i + 1][j + 1][k]);
			}
		}
	}
	const int lcslen = matrix[len1][len2][len3];
	dest[lcslen] = '\0';
	int heimax = len1 - 1, rowmax = len2 - 1, colmax = len3 - 1;
	for (int i = lcslen - 1; i >= 0; i--) {
		for (int hei = heimax; hei >= 0; hei--) {
			for (int row = rowmax; row >= 0; row--) {
				for (int col = colmax; col >= 0; col--) {
					if ((i + 1) == matrix[hei + 1][row + 1][col + 1] && EQUAL3(str1[hei], str2[row], str3[col])) {
						dest[i] = str1[hei];
						heimax = hei - 1, rowmax = row - 1, colmax = col - 1;
						goto BREAK;
					}
				}
			}
		} BREAK:;
	}
	return dest;
}