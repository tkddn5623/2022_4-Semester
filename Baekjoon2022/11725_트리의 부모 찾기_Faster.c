#include <stdio.h>
#include <stdlib.h>
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
typedef int Element;
typedef struct {
	Element* items;
	int capacity;
	int front;
	int rear;
} ArrayQueue;
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
	Graph* graph;
	if ((graph = malloc(sizeof(Graph))) == NULL) exit(1);
	if ((graph->tails = calloc(vtxsize, sizeof(GNode*))) == NULL) exit(1);
	if ((graph->_edges = calloc(edgesize + vtxsize, sizeof(GNode))) == NULL) exit(1);
	graph->vtxsize = vtxsize;
	graph->edgesize = edgesize + vtxsize;
	graph->edgecount = vtxsize;
	for (int i = 0; i < vtxsize; i++) {
		graph->tails[i] = &graph->_edges[i];
		graph->tails[i]->next = graph->tails[i];
	}
	return graph;
}
void GR_deleteGraph(Graph* graph) {
	free(graph->_edges);
	free(graph->tails);
	free(graph);
}
void GR_addEdge(Graph* graph, const int from, const int to) {
	graph->_edges[graph->edgecount].id = to;
	graph->_edges[graph->edgecount].next = graph->tails[from]->next;
	graph->tails[from]->next = &graph->_edges[graph->edgecount];
	graph->tails[from] = &graph->_edges[graph->edgecount++];
}
void GR_BFS(Graph* pgraph, const int startIdx) {
	ArrayQueue* queue = AQ_newQueue(pgraph->vtxsize);
	int* visited = calloc(pgraph->vtxsize, sizeof(int));
	if (!visited) exit(1);
	AQ_push(queue, startIdx);
	while (!AQ_isEmpty(queue)) {
		int vtx = AQ_pop(queue);
		GNode* const head = &pgraph->_edges[vtx];
		for (GNode* cur = head->next; cur != head; cur = cur->next) {
			int id = cur->id;
			if (!visited[id]) {
				visited[id] = 1;
				pgraph->_edges[id].id = vtx;
				AQ_push(queue, id);
			}
		}
	}
	AQ_deleteQueue(queue);
	free(visited);
}
int main() {
	int N;
	scanf("%d", &N);
	Graph* graph = GR_newGraph(N + 1, 2 * (N - 1));
	for (int i = 0; i < N - 1; i++) {
		int a, b;
		scanf("%d%d", &a, &b);
		GR_addEdge(graph, a, b);
		GR_addEdge(graph, b, a);
	}
	GR_BFS(graph, 1);
	for (int i = 2; i <= N; i++) {
		printf("%d\n", graph->_edges[i].id);
	}
	GR_deleteGraph(graph);
}