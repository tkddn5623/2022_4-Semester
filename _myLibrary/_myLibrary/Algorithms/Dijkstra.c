#include <stdlib.h>
typedef struct _GNode {
	int from;
	int to;
	int weight;
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** tails;
} Graph;
Graph * GR_newGraph(const int size) {
	Graph* pgraph;
	if ((pgraph = malloc(sizeof(Graph))) == NULL) exit(1);
	if ((pgraph->tails = calloc(size, sizeof(GNode*))) == NULL) exit(1);
	pgraph->size = size;
	for (int i = 0; i < size; i++) {
		pgraph->tails[i] = malloc(sizeof(GNode));
		pgraph->tails[i]->to = 0;
		pgraph->tails[i]->next = pgraph->tails[i];
	}
	return pgraph;
}
void GR_deleteGraph(Graph* pgraph) {
	const int size = pgraph->size;
	for (int i = 0; i < size; i++) {
		GNode* _tail = pgraph->tails[i];
		GNode* cur = _tail;
		do {
			GNode* temp = cur;
			cur = cur->next;
			free(temp);
		} while (cur != _tail);
	}
	free(pgraph->tails);
	free(pgraph);
}
GNode* GR_addEdge(const Graph* pgraph, const int from, const int to) {
	GNode * newNode = malloc(sizeof(GNode));
	newNode->to = to;
	newNode->from = from;
	newNode->next = pgraph->tails[from]->next;
	pgraph->tails[to]->next = newNode;
	pgraph->tails[to] = newNode;
	pgraph->tails[from]->next = newNode;
	pgraph->tails[from] = newNode;
	return newNode;
}