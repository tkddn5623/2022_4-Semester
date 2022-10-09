#include <stdio.h>
#include <stdlib.h>
typedef struct _GNode {
	int from;
	int to;
	int weight;
	//struct _GNode* next;
} GNode;
int _cmp(const void* _a, const void* _b) {
	if (((GNode*)_a)->weight > ((GNode*)_b)->weight) return 1;
	else if (((GNode*)_a)->weight < ((GNode*)_b)->weight) return -1;
	else return 0;
}
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
int getWeight(const GNode* edges, const int vtxSize, const int edgeSize) {
	int weight = 0;
	int* roots = UF_init(vtxSize + 1);
	for (int i = 0; i < edgeSize; i++) {
		GNode edge_i = edges[i];
		if (UF_find(roots, edge_i.from) != UF_find(roots, edge_i.to)) {
			UF_union(roots, edge_i.from, edge_i.to);
			weight += edge_i.weight;
		}
	}
	free(roots);
	return weight;
}
int main() {
	/*{
		freopen("i.txt", "r", stdin);
	}*/
	GNode* edges;
	int V, E;

	scanf("%d%d", &V, &E);
	if ((edges = calloc(E, sizeof(GNode))) == NULL) exit(1);
	for (int i = 0; i < E; i++) {
		scanf("%d%d%d", &edges[i].from, &edges[i].to, &edges[i].weight);
	}
	qsort(edges, E, sizeof(GNode), _cmp);
	printf("%d\n", getWeight(edges, V, E));
	free(edges);
}