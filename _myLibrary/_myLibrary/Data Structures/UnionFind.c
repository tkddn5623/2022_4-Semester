#include <stdlib.h>
#include "UnionFind.h"
int* UF_init(const int maxsize) {
	int* roots;
	if ((roots = calloc(maxsize, sizeof(int))) == NULL) exit(1);
	for (int i = 0; i < maxsize; i++)
		roots[i] = i;
	return roots;
}
int UF_find(int* roots, const int key) {
	if (roots[key] == key) return key;
	else return roots[key] = UF_find(roots, roots[key]);
}
void UF_union(int* roots, int A, int B) {
	A = UF_find(roots, A);
	B = UF_find(roots, B);
	roots[A] = B;
}



/*
* 2022.9.5 Mon
*/