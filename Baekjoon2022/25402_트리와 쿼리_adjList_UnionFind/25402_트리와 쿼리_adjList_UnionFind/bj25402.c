#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
	GNode** heads;
	GNode** tails;
} Graph;
int UF_find(int* roots, const int key) {
	if (roots[key] == key) return key;
	else return roots[key] = UF_find(roots, roots[key]);
}
void UF_union(int* roots, int A, int B, int* ranks) {
	A = UF_find(roots, A);
	B = UF_find(roots, B);
	if (A == B)return;
	roots[A] = B;
	ranks[B] += ranks[A];
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
void GR_addEdge(const Graph* pgraph, const int index1, const int index2) {
	GNode* newNode1, * newNode2, * cur;
	newNode1 = malloc(sizeof(GNode));
	newNode2 = malloc(sizeof(GNode));
	newNode1->id = index1;
	newNode1->next = NULL;
	cur = pgraph->tails[index2];
	cur->next = newNode1;
	pgraph->tails[index2] = newNode1;
	newNode2->id = index2;
	newNode2->next = NULL;
	cur = pgraph->tails[index1];
	cur->next = newNode2;
	pgraph->tails[index1] = newNode2;

}
int GR_BFS(Graph* pgraph, int* visited, const int startIdx) {
	const int size = pgraph->size;
	int count = 0;
	ArrayQueue* queue = AQ_newQueue(size);
	if (!visited[startIdx]) {
		AQ_push(queue, startIdx);
		visited[startIdx] = 1;
		pgraph->heads[startIdx]->id = startIdx;
	}
	while (!AQ_isEmpty(queue)) {
		GNode* cur;
		int vtx = AQ_pop(queue);
		cur = pgraph->heads[vtx]->next;
		while (cur != NULL) {
			if (!visited[cur->id]) {
				AQ_push(queue, cur->id);
				visited[cur->id] = 1;
				pgraph->heads[cur->id]->id = vtx;
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
	GR_BFS(pgraph, visited, 1);
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
			int picksPrev = pgraph->heads[picks[j]]->id;
			if (selected[picksPrev]) {
				UF_union(roots, picks[j], picksPrev, ranks);
			}
		}
		for (int j = 0; j < K; j++) {
			if (!visited[picks[j]] && UF_find(roots, picks[j]) == picks[j]) {
				printf("Now r os %d\n", ranks[picks[j]]);
				answer += (long long int)ranks[picks[j]] * (ranks[picks[j]] - 1) / 2;
				printf("Now a os %d\n", answer);
				visited[picks[j]] = 1;
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
	{
		freopen("example.txt", "r", stdin);
	}
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