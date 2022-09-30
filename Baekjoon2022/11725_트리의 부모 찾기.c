#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
typedef struct _GNode {
	int id;
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** heads;
	GNode** tails;
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
int AQ_size(const ArrayQueue* pqueue) {
	const int subtract = pqueue->rear - pqueue->front;
	if (subtract >= 0) return subtract;
	else return pqueue->capacity + subtract;
}
Element AQ_front(const ArrayQueue* pqueue) {
	return pqueue->items[pqueue->front];
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
void GR_BFS(Graph* pgraph, const int startIdx) {
	const int size = pgraph->size;
	ArrayQueue* queue = AQ_newQueue(size);
	int* visited = calloc(size, sizeof(int));
	AQ_push(queue, startIdx);
	while (!AQ_isEmpty(queue)) {
		GNode* cur;
		int vtx = AQ_pop(queue);
		cur = pgraph->heads[vtx]->next;
		while (cur != NULL) {
			if (!visited[cur->id]) {
				visited[cur->id] = 1;
				pgraph->heads[cur->id]->id = vtx;
				AQ_push(queue, cur->id);
			}
			cur = cur->next;
		}
	}
	AQ_deleteQueue(queue);
	free(visited);
}
int main() {
	int N;
	scanf("%d", &N);
	Graph* graph = GR_newGraph(N + 1);
	for (int i = 0; i < N - 1; i++) {
		int a, b;
		scanf("%d%d", &a, &b);
		GR_addEdges(graph, a, b);
	}
	GR_BFS(graph, 1);
	for (int i = 2; i <= N; i++) {
		printf("%d\n", graph->heads[i]->id);
	}
	GR_deleteGraph(graph);
}