#include <stdlib.h>
#include "AdjacencyList.h"
/*
typedef struct _GNode {
	int id;
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** tails;
} Graph;
*/
Graph* GR_newGraph(const int size) {
	Graph* pgraph;
	if ((pgraph = malloc(sizeof(Graph))) == NULL) exit(1);
	if ((pgraph->tails = calloc(size, sizeof(GNode*))) == NULL) exit(1);
	pgraph->size = size;
	for (int i = 0; i < size; i++) {
		pgraph->tails[i] = malloc(sizeof(GNode));
		pgraph->tails[i]->id = 0;
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
void GR_addEdge(const Graph* pgraph, const int index1, const int index2) {
	GNode* newNode1, * newNode2;
	newNode1 = malloc(sizeof(GNode));
	newNode2 = malloc(sizeof(GNode));
	newNode1->id = index1;
	newNode1->next = pgraph->tails[index2]->next;
	pgraph->tails[index2]->next = newNode1;
	pgraph->tails[index2] = newNode1;
	newNode2->id = index2;
	newNode2->next = pgraph->tails[index1]->next;
	pgraph->tails[index1]->next = newNode2;
	pgraph->tails[index1] = newNode2;
}
/*int GR_BFS(Graph* pgraph, const int startIdx) {
	const int size = pgraph->size;
	int count = 0;
	int* visited = calloc(size, sizeof(int));
	ArrayQueue* queue = AQ_newQueue(size);
	if (!visited[startIdx]) {
		AQ_push(queue, startIdx);
		visited[startIdx] = 1;
		pgraph->tails[startIdx]->next->id = startIdx; //If it is tree, this stores the 'parent' index.
	}
	while (!AQ_isEmpty(queue)) {
		int vtx = AQ_pop(queue);
		GNode* _head = pgraph->tails[vtx]->next;
		GNode* cur = _head->next;
		while (cur != _head) {
			if (!visited[cur->id]) {
				AQ_push(queue, cur->id);
				visited[cur->id] = 1;
				praph->tails[cur->id]->next->id = vtx; //If it is tree, this stores the 'parent' index.
				count++;
			}
			cur = cur->next;
		}
	}
	AQ_deleteQueue(queue);
	return count;
}
void GR_addEdge(const Graph* pgraph, const int index_from, const int index_to) {
	GNode* newNode;
	newNode = malloc(sizeof(GNode));
	newNode->id = index_to;
	newNode->next = pgraph->tails[index_from]->next;
	pgraph->tails[index_from]->next = newNode;
	pgraph->tails[index_from] = newNode;
}
*/

/*
* 2022.9.21 Wed
*/