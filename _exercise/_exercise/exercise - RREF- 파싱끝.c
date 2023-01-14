#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXBUF (200)
#define MAXDIM (5)
typedef struct _fraction_t {
	int numer;
	int denom;
} fraction_t;
void printmat(fraction_t mat[][MAXDIM], const int row, const int col) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) { printf("%d/%d ", mat[i][j].numer, mat[i][j].denom); }
		putchar('\n');
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
					printmat(mat, rowsize, colsize);
					pivot_num++;
					exit(1); //DEBUG;
					goto ESC;
				}
			}
		} ESC:;
		if (pivot_done) break;
		
	}

}
int main() {
	{
		freopen("i.txt", "r", stdin);
	}
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
}