#include <stdlib.h>
#include "AdjacencyList.h"
/*
typedef struct _GNode {
	int id;
	int weight;
	struct _GNode* next;
} GNode;
typedef struct {
	int vtxsize;
	int edgesize;
	int edgecount;
	GNode** tails;
	GNode* _edges;
} Graph;
*/
Graph* GR_newGraph(const int vtxsize, const int edgesize) {
	Graph* pgraph;
	if ((pgraph = malloc(sizeof(Graph))) == NULL) exit(1);
	if ((pgraph->tails = calloc(vtxsize, sizeof(GNode*))) == NULL) exit(1);
	if ((pgraph->_edges = calloc(edgesize + vtxsize, sizeof(GNode))) == NULL) exit(1);
	pgraph->vtxsize = vtxsize;
	pgraph->edgesize = edgesize + vtxsize;
	pgraph->edgecount = vtxsize;
	for (int i = 0; i < vtxsize; i++) {
		pgraph->tails[i] = &pgraph->_edges[i];
		pgraph->tails[i]->next = pgraph->tails[i];
	}
	return pgraph;
}
void GR_deleteGraph(Graph* pgraph) {
	free(pgraph->_edges);
	free(pgraph->tails);
	free(pgraph);
}
void GR_addEdge(Graph* pgraph, const int from, const int to, const int weight) {
	pgraph->_edges[pgraph->edgecount].id = to;
	pgraph->_edges[pgraph->edgecount].weight = weight;
	pgraph->_edges[pgraph->edgecount].next = pgraph->tails[from]->next;
	pgraph->tails[from]->next = &pgraph->_edges[pgraph->edgecount];
	pgraph->tails[from] = &pgraph->_edges[pgraph->edgecount++];
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
				pgraph->tails[cur->id]->next->id = vtx; //If it is tree, this stores the 'parent' index.
				count++;
			}
			cur = cur->next;
		}
	}
	free(visited);
	AQ_deleteQueue(queue);
	return count;
}*/

/*
* 2022.9.21 Wed
* 2023.1.6  Fri, improved speed.
*/