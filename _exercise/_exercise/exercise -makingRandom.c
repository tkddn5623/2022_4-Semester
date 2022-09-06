#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
	GNode** heads;
} Graph;
ArrayQueue * AQ_newQueue(const int max) {
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
	pgraph->size = size;
	for (int i = 0; i < size; i++)
		pgraph->heads[i] = calloc(1, sizeof(GNode));
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
	cur = pgraph->heads[index2];
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = newNode1;
	newNode2->id = index2;
	newNode2->next = NULL;
	cur = pgraph->heads[index1];
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = newNode2;
}
int GR_DFS(const Graph* pgraph, int* visited, const int startIdx) {
	const int size = pgraph->size;
	int count = 0;
	ArrayQueue* queue = AQ_newQueue(size);
	if (!visited[startIdx]) {
		visited[startIdx] = 1;
		AQ_push(queue, startIdx);
	}
	while (!AQ_isEmpty(queue)) {
		GNode* cur;
		int vtx = AQ_pop(queue);
		cur = pgraph->heads[vtx]->next;
		while (cur != NULL) {
			if (!visited[cur->id]) {
				AQ_push(queue, cur->id);
				visited[cur->id] = 1;
				count++;
			}
			cur = cur->next;
		}
	}
	AQ_deleteQueue(queue);
	return count;
}
void getQuery(Graph* pgraph, const int Q) {
	const int N = pgraph->size;
	int* visited = calloc(N, sizeof(int));
	for (int i = 0; i < Q; i++) {
		int K, answer = 0;
		//scanf("%d", &K);
		K = rand() % (N - 1) + 1;
		printf("%d ", K);
		for (int j = 1; j <= N; j++) {
			visited[j] = 1;
		}
		for (int j = 0; j < K; j++) {
			int temp;
			//scanf("%d", &temp);
			do {
				temp = rand() % (N-1) + 1;
			} while (!visited[temp]);
			printf("%d ", temp);
			visited[temp] = 0;
		}
		printf("\n.");
		for (int j = 1; j <= N; j++) {
			int searchCount = 0;
			if (!visited[j]) searchCount = GR_DFS(pgraph, visited, j);
			answer += searchCount * (searchCount + 1) / 2;
		}
		printf("%d\n", answer);
	}
}
/*int* UF_init(const int maxsize) {
	int* roots;
	if ((roots = calloc(maxsize, sizeof(int))) == NULL) exit(1);
	for (int i = 0; i < maxsize; i++)
		roots[i] = i;
	return roots;
}
int UF_find(int* roots, const int key) {
	if (roots[key] == key) return key;
	else return roots[key] = UF_find(roots, roots[key]);
	return 0;
}
void UF_union(int* roots, int A, int B) {
	A = UF_find(roots, A);
	B = UF_find(roots, B);
	roots[A] = B;
}*/
int main() {
	{
		//freopen("i.txt", "r", stdin);
		srand(11);
	}
	int N, Q;
	//int* roots;
	scanf("%d", &N);
	Graph* graph = GR_newGraph(N + 1);
	//if ((roots = calloc(N + 1, sizeof(int))) == NULL) exit(1);
	for (int i = 0; i < N - 1; i++) {
		int a, b;
		//scanf("%d%d", &a, &b);
		a = rand() % N + 1;
		do {
			b = rand() % N + 1;
		} while (a == b);
		printf("%d %d\n", a, b);
		GR_addEdge(graph, a, b);
	}
	scanf("%d", &Q);
	getQuery(graph, Q);
}