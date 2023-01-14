#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXBUF (200)
#define MAXDIM (10)
typedef struct _fraction_t {
	int numer;
	int denom;
} fraction_t;
int gcd(int A, int B) {
	int C;
	if (A < 0) A = -A;
	if (B < 0) B = -B;
	while (B > 0) { C = A % B; A = B; B = C; }
	return A;
}
void _printmat_debug(fraction_t mat[][MAXDIM], const int row, const int col) {
	printf("--------\n");
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) { printf("%d/%d ", mat[i][j].numer, mat[i][j].denom); }
		putchar('\n');
	}
	printf("--------\n");
}
void printmat(fraction_t mat[][MAXDIM], const int row, const int col) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) { 
			if (mat[i][j].denom != 1) printf("%d/%d ", mat[i][j].numer, mat[i][j].denom);
			else printf("%d ", mat[i][j].numer);
		}
		putchar('\n');
	}
}
fraction_t fracRecip(fraction_t fr) {
	fraction_t recip;
	if (fr.numer == 0) { 
		printf("Error: NaN occurs\n");
		exit(1); 
	}
	else if (fr.numer > 0) {
		recip.numer = fr.denom;
		recip.denom = fr.numer;
	}
	else {
		recip.numer = -fr.denom;
		recip.denom = -fr.numer;
	}
	return recip;
}
fraction_t fracReduct(fraction_t fr) {
	/*if (fr.numer == 0) {
		fr.denom = 1;
	}
	else {
		const int g = gcd(fr.numer, fr.denom);
		fr.numer = fr.numer / g;
		fr.denom = fr.denom / g;
	}*/
	const int g = gcd(fr.numer, fr.denom);
	fr.numer = fr.numer / g;
	fr.denom = fr.denom / g;
	return fr;
}
void rowScaling(fraction_t dest[], fraction_t scale, int colsize) {
	for (int i = 0; i < colsize; i++) {
		fraction_t fr = dest[i];
		fr.numer *= scale.numer;
		fr.denom *= scale.denom;
		dest[i] = fracReduct(fr);
	}
}
void rowReplace(fraction_t dest[], fraction_t src[], fraction_t scale, int colsize) {
	for (int i = 0; i < colsize; i++) {
		fraction_t fr;
		fr.numer = scale.denom * src[i].denom * dest[i].numer + scale.numer * src[i].numer * dest[i].denom;
		fr.denom = scale.denom * src[i].denom * dest[i].denom;
		dest[i] = fracReduct(fr);
	}
}
void makeRREF(fraction_t mat[][MAXDIM], const int rowsize, const int colsize) {
	int pivot_lastcol = -1;
	int pivot_num = 0;
	for (int _i = 0; _i < rowsize; _i++) {
		int pivot_done = 1;
		for (int c = pivot_lastcol + 1; c < colsize; c++) {
			for (int r = pivot_num; r < rowsize; r++) {
				if (mat[r][c].numer != 0) {
					pivot_done = 0;
					pivot_lastcol = c;
					if (pivot_num != r) {
						for (int i = 0; i < colsize; i++) {
							fraction_t temp = mat[pivot_num][i]; //Row interchange
							mat[pivot_num][i] = mat[r][i];
							mat[r][i] = temp;
						}
					}
					if (mat[pivot_num][pivot_lastcol].numer != 1 || mat[pivot_num][pivot_lastcol].denom != 1) {
						rowScaling(mat[pivot_num], fracRecip(mat[pivot_num][pivot_lastcol]), colsize);
					}
					pivot_num++;
					goto ESC;
				}
			}
		} ESC:;
		if (pivot_done) break;
		for (int r = 0; r < rowsize; r++) {
			if (r == pivot_num - 1) continue;
			if (mat[r][pivot_lastcol].numer != 0) {
				fraction_t weight = mat[r][pivot_lastcol];
				weight.numer = -weight.numer;
				rowReplace(mat[r], mat[pivot_num - 1], weight, colsize);
			}
			
		}
	}
	
}
int main() {
	char buffer[MAXBUF];
	int row, col;
	fraction_t mat[MAXDIM][MAXDIM] = { 0 };
	scanf("%d%d\n", &row, &col);
	for (int i = 0; i < row; i++) { //Parsing
		int j = 0;
		char* bp;
		fgets(buffer, sizeof(buffer), stdin);
		if ((bp = strchr(buffer, '\n')) != NULL) *bp = '\0';
		bp = strtok(buffer, " ");
		if (strchr(bp, '/')) sscanf(bp, "%d/%d", &mat[i][j].numer, &mat[i][j].denom);
		else { sscanf(bp, "%d", &mat[i][j].numer); mat[i][j].denom = 1; }
		j++;
		while ((bp = strtok(NULL, " ")) != NULL) {
			if (strchr(bp, '/')) sscanf(bp, "%d/%d", &mat[i][j].numer, &mat[i][j].denom);
			else { sscanf(bp, "%d", &mat[i][j].numer); mat[i][j].denom = 1; }
			j++;
		}
	}
	makeRREF(mat, row, col);
	printmat(mat, row, col);
}