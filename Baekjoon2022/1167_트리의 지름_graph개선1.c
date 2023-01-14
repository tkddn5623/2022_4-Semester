#include <stdio.h>
#include <stdlib.h>
typedef int Element;
typedef struct {
	Element* items;
	int capacity;
	int front;
	int rear;
} ArrayQueue;
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
		pgraph->tails[i]->id = -1;
		pgraph->tails[i]->weight = -1;
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
	GNode e;
	e.id = to;
	e.weight = weight;
	e.next = pgraph->tails[from]->next;
	pgraph->_edges[pgraph->edgecount] = e;
	pgraph->tails[from]->next = &pgraph->_edges[pgraph->edgecount];
	pgraph->tails[from] = &pgraph->_edges[pgraph->edgecount++];
}
ArrayQueue* AQ_newQueue(const int max) {
	ArrayQueue* pqueue;
	if ((pqueue = malloc(sizeof(ArrayQueue))) == NULL) exit(1);
	if ((pqueue->items = calloc(max + 1, sizeof(Element))) == NULL) exit(1);
	pqueue->capacity = max + 1;
	pqueue->front = 0;
	pqueue->rear = 0;
	return pqueue;
}
void AQ_deleteQueue(ArrayQueue* pqueue) {
	if (pqueue == NULL || pqueue->items == NULL) exit(1);
	free(pqueue->items);
	free(pqueue);
}
void AQ_flush(ArrayQueue* pqueue) {
	pqueue->front = 0;
	pqueue->rear = 0;
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
int treeDiameter(Graph* pgraph) {
	const int vtxsize = pgraph->vtxsize;
	int* visited = calloc(vtxsize, sizeof(int));
	int* dist = calloc(vtxsize, sizeof(int));
	int distmax = 0;
	int distmax_vtx = 0;
	ArrayQueue* queue = AQ_newQueue(vtxsize);

	AQ_push(queue, 1);
	visited[1] = 1;
	dist[1] = 0;
	while (!AQ_isEmpty(queue)) {
		int vtx = AQ_pop(queue);
		GNode* _head = pgraph->tails[vtx]->next;
		GNode* cur = _head->next;
		while (cur != _head) {
			int cur_id = cur->id;
			int cur_weight = cur->weight;
			if (!visited[cur_id]) {
				AQ_push(queue, cur_id);
				visited[cur_id] = 1;
				dist[cur_id] = dist[vtx] + cur_weight;
				if (distmax < dist[cur_id]) {
					distmax = dist[cur_id];
					distmax_vtx = cur_id;
				}
			}
			cur = cur->next;
		}
	}
	for (int i = 0; i < vtxsize; i++) {
		visited[i] = 0;
	}
	dist[distmax_vtx] = 0;
	visited[distmax_vtx] = 1;
	AQ_flush(queue);
	AQ_push(queue, distmax_vtx);

	while (!AQ_isEmpty(queue)) {
		int vtx = AQ_pop(queue);
		GNode* _head = pgraph->tails[vtx]->next;
		GNode* cur = _head->next;
		while (cur != _head) {
			int cur_id = cur->id;
			int cur_weight = cur->weight;
			if (!visited[cur_id]) {
				AQ_push(queue, cur_id);
				visited[cur_id] = 1;
				dist[cur_id] = dist[vtx] + cur_weight;
				if (distmax < dist[cur_id]) {
					distmax = dist[cur_id];
					distmax_vtx = cur_id;
				}
			}
			cur = cur->next;
		}
	}
	free(visited);
	free(dist);
	AQ_deleteQueue(queue);
	return distmax;
}
int main() {
	int V;
	Graph* g;
	scanf("%d", &V);
	g = GR_newGraph(V + 1, 2 * V - 2);
	for (int i = 0; i < V; i++) {
		int vtx;
		scanf("%d", &vtx);
		do {
			int to, dist;
			scanf("%d", &to);
			if (to == -1) break;
			scanf("%d", &dist);
			GR_addEdge(g, vtx, to, dist);
		} while (1);
	}
	printf("%d\n", treeDiameter(g));
}
