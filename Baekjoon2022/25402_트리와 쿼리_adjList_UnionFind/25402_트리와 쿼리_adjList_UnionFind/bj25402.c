//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int size;
	GNode** tails;
} Graph;
int UF_find(int* roots, const int key) {
	if (roots[key] == key) return key;
	else return roots[key] = UF_find(roots, roots[key]);
}
long long int UF_union(int* roots, int A, int B, int* ranks) {
	long long int ret = 0;
	A = UF_find(roots, A);
	B = UF_find(roots, B);
	if (A == B) return 0;
	roots[A] = B;
	ret = (long long int)ranks[A] * (long long int)ranks[B];
	ranks[B] += ranks[A];
	return ret;
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

int GR_BFS(Graph* pgraph, int* visited, const int startIdx, int* prevs) {
	const int size = pgraph->size;
	int count = 0;
	ArrayQueue* queue = AQ_newQueue(size);
	if (!visited[startIdx]) {
		AQ_push(queue, startIdx);
		visited[startIdx] = 1;
		pgraph->tails[startIdx]->next->id = startIdx;
	}
	while (!AQ_isEmpty(queue)) {
		int vtx = AQ_pop(queue);
		GNode* _head = pgraph->tails[vtx]->next;
		GNode* cur = _head->next;
		while (cur != _head) {
			if (!visited[cur->id]) {
				AQ_push(queue, cur->id);
				visited[cur->id] = 1;
				prevs[cur->id] = vtx;
				count++;
			}
			cur = cur->next;
		}
	}
	AQ_deleteQueue(queue);
	return count;
}
void getQuery(Graph* pgraph, const int Q) {
	const int size = pgraph->size;
	int* visited = calloc(size, sizeof(int));
	int* prevs = calloc(size, sizeof(int));
	GR_BFS(pgraph, visited, 1, prevs);
	memset(visited, 0, size * sizeof(int)); //reuse.
	int* selected = calloc(size, sizeof(int));
	int* picks = calloc(size, sizeof(int));
	int* roots = calloc(size, sizeof(int));
	int* ranks = calloc(size, sizeof(int));
	for (int i = 1; i < size; i++) {
		roots[i] = i;
		ranks[i] = 1;
	}
	for (int i = 0; i < Q; i++) {
		int K;
		long long int answer = 0;

		scanf("%d", &K);
		for (int j = 0; j < K; j++) {
			scanf("%d", &picks[j]);
			selected[picks[j]] = 1;
		}
		for (int j = 0; j < K; j++) {
			int picksPrev = prevs[picks[j]];
			if (selected[picksPrev]) {
				answer += UF_union(roots, picks[j], picksPrev, ranks);
			}
		}
		for (int j = 0; j < K; j++) {
			visited[picks[j]] = 0;
			selected[picks[j]] = 0;
			roots[picks[j]] = picks[j];
			ranks[picks[j]] = 1;
		}
		printf("%lld\n", answer);
	}
	//free(picks);
}
int main() {
	int N, Q;
	scanf("%d", &N);
	Graph* graph = GR_newGraph(N + 1);
	for (int i = 0; i < N - 1; i++) {
		int a, b;
		scanf("%d%d", &a, &b);
		GR_addEdge(graph, a, b);
	}
	scanf("%d", &Q);
	getQuery(graph, Q);
}