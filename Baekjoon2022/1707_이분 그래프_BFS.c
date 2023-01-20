#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXVTX (20000)
#define MAXEDGE (400000)
typedef int Element;
typedef struct {
	Element* items;
	int capacity;
	int front;
	int rear;
} ArrayQueue;
typedef struct _GNode {
	int id;
	struct _GNode* next;
} GNode;
typedef struct {
	int vtxsize;
	int edgesize;
	int edgecount;
	GNode** tails;
	GNode* _edges;
} Graph;
ArrayQueue* AQ_newQueue(const int max) {
	ArrayQueue* pqueue;
	if ((pqueue = malloc(sizeof(ArrayQueue))) == NULL) exit(1);
	if ((pqueue->items = calloc(max + 1, sizeof(Element))) == NULL) exit(1);
	pqueue->capacity = max + 1;
	pqueue->front = 0;
	pqueue->rear = 0;
	return pqueue;
}
ArrayQueue* AQ_flush(ArrayQueue* pqueue) {
	pqueue->front = 0;
	pqueue->rear = 0;
	return pqueue;
}
void AQ_deleteQueue(ArrayQueue* pqueue) {
	if (pqueue == NULL || pqueue->items == NULL) exit(1);
	free(pqueue->items);
	free(pqueue);
}
int AQ_isEmpty(const ArrayQueue* pqueue) {
	return pqueue->front == pqueue->rear;
}
void AQ_push(ArrayQueue* pqueue, const Element item) {
	const int rear = pqueue->rear;
	pqueue->items[rear] = item;
	pqueue->rear = (rear + 1) % pqueue->capacity;
}
Element AQ_pop(ArrayQueue* pqueue) {
	const int front = pqueue->front;
	pqueue->front = (front + 1) % pqueue->capacity;
	return pqueue->items[front];
}
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
Graph* GR_flush(Graph* pgraph) {
	const int vtxsize = pgraph->vtxsize;
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
void GR_addEdge(Graph* pgraph, const int vtx1, const int vtx2) {
	pgraph->_edges[pgraph->edgecount].id = vtx2;
	pgraph->_edges[pgraph->edgecount].next = pgraph->tails[vtx1]->next;
	pgraph->tails[vtx1]->next = &pgraph->_edges[pgraph->edgecount];
	pgraph->tails[vtx1] = &pgraph->_edges[pgraph->edgecount++];

	pgraph->_edges[pgraph->edgecount].id = vtx1;
	pgraph->_edges[pgraph->edgecount].next = pgraph->tails[vtx2]->next;
	pgraph->tails[vtx2]->next = &pgraph->_edges[pgraph->edgecount];
	pgraph->tails[vtx2] = &pgraph->_edges[pgraph->edgecount++];
}
int isBipartite(Graph* graph, ArrayQueue* queue) {
	const int vtxsize = graph->vtxsize;
	static visited[MAXVTX + 1];
	memset(visited, 0, sizeof(visited));
	for (int i = 1; i < vtxsize; i++) {
		if (visited[i]) continue;
		AQ_push(queue, i);
		visited[i] = 1;
		while (!AQ_isEmpty(queue)) {
			int vtx = AQ_pop(queue);
			int color = visited[vtx];
			GNode* _head = graph->tails[vtx]->next;
			GNode* cur = _head->next;
			while (cur != _head) {
				//if (cur->id == vtx) { cur = cur->next; continue; }
				//This self-cycle check makes program very SLOW. (308MS -> 352MS)
				if (!visited[cur->id]) {
					AQ_push(queue, cur->id);
					visited[cur->id] = (color ^ 0x3);
				}
				else if (visited[cur->id] == color) {
					return 0;
				}
				cur = cur->next;
			}
		}
	}
	return 1;
}
int main() {
	Graph* graph = GR_newGraph(MAXVTX + 1, MAXEDGE);
	ArrayQueue* queue = AQ_newQueue(MAXEDGE);
	int _tc;
	int V, E;
	scanf("%d", &_tc);
	while (_tc--) {
		int vtx1, vtx2;
		scanf("%d%d", &V, &E);
		for (int i = 0; i < E; i++) {
			scanf("%d%d", &vtx1, &vtx2);
			GR_addEdge(graph, vtx1, vtx2);
		}
		switch (isBipartite(graph, queue)) {
		case 0: printf("NO\n"); break;
		case 1: printf("YES\n"); break;
		}
		AQ_flush(queue);
		GR_flush(graph);
	}
	AQ_deleteQueue(queue);
	GR_deleteGraph(graph);
}