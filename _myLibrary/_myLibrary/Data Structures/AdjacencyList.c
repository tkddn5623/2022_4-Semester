#include <stdlib.h>
#include "AdjacencyList.h"
/*
typedef struct _GNode {
	int id;
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** heads;
	GNode** tails;
} Graph;
*/
Graph* GR_newGraph(const int size) {
	Graph* pgraph;
	if ((pgraph = malloc(sizeof(Graph))) == NULL) exit(1);
	if ((pgraph->heads = calloc(size, sizeof(GNode*))) == NULL) exit(1);
	if ((pgraph->tails = calloc(size, sizeof(GNode*))) == NULL) exit(1);
	pgraph->size = size;
	for (int i = 0; i < size; i++) {
		pgraph->heads[i] = calloc(1, sizeof(GNode));
		pgraph->tails[i] = pgraph->heads[i];
	}
	return pgraph;
}
void GR_deleteGraph(Graph* pgraph) {
	const int size = pgraph->size;
	for (int i = 0; i < size; i++) {
		GNode* cur = pgraph->heads[i];
		while (cur != NULL) {
			GNode* temp = cur;
			cur = cur->next;
			free(temp);
		}
	}
	free(pgraph->heads);
	free(pgraph);
}
void GR_addEdges(const Graph* pgraph, const int index1, const int index2) {
	GNode* newNode1, * newNode2;
	newNode1 = malloc(sizeof(GNode));
	newNode2 = malloc(sizeof(GNode));
	newNode1->id = index1;
	newNode1->next = NULL;
	pgraph->tails[index2]->next = newNode1;
	pgraph->tails[index2] = newNode1;
	newNode2->id = index2;
	newNode2->next = NULL;
	pgraph->tails[index1]->next = newNode2;
	pgraph->tails[index1] = newNode2;
}



/*
* 2022.9.7 Wed
*/