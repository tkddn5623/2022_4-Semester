#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
double RUNTIME;
double ENDTIME;
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
int UF_find(int* roots, const int key) {
	if (roots[key] == key) return key;
	else return roots[key] = UF_find(roots, roots[key]);
}
void UF_union(int* roots, int A, int B, int* ranks, int answer) {
	A = UF_find(roots, A);
	B = UF_find(roots, B);
	if (A == B)return;
	roots[A] = B;
	ranks[B] = ranks[A] + ranks[B];
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
	int totalk = 0;
	double time11 = 0, time22 = 0, time33 = 0, time44 = 0;
	const int size = pgraph->size;
	ArrayQueue* queue = AQ_newQueue(size);
	int* visited = calloc(size, sizeof(int));
	GR_BFS(pgraph, visited, 1);
	free(visited);
	int* selected = calloc(size, sizeof(int));
	int* UFroot = calloc(size, sizeof(int));
	int* UFrank = calloc(size, sizeof(int));
	for (int j = 1; j < size; j++) {
		UFroot[j] = j;
		UFrank[j] = 0;
	}
	for (int i = 0; i < Q; i++) {
		double time1 = (double)clock() / CLOCKS_PER_SEC;
		int K, answer = 0;
		scanf("%d", &K);
		totalk += K;
		memset(selected, 0, size * sizeof(int));
		
		for (int j = 0; j < K; j++) {
			int temp;
			scanf("%d", &temp);
			//visited[temp] = 0;
			selected[temp] = 1;
			UFrank[temp] = 1;
			AQ_push(queue, temp);
		}
		
		double time2 = (double)clock() / CLOCKS_PER_SEC;
		for (int j = 0; j < K; j++) {
			int vtx = AQ_pop(queue);
			AQ_push(queue, vtx);
			int prev = pgraph->heads[vtx]->id;
			if (selected[prev]) UF_union(UFroot, vtx, prev, UFrank, answer);
		}
		
		double time3 = (double)clock() / CLOCKS_PER_SEC;
		for (int j = 0; j < K; j++) {
			int pop = AQ_pop(queue);
			AQ_push(queue, pop);
			int vtx = UF_find(UFroot, pop);
			if (selected[vtx]) {
				selected[vtx] = 0;
				answer += UFrank[vtx] * (UFrank[vtx] - 1) / 2;
			}
			
		}
		
		for (int j = 0; j < K; j++) {
			int vtx = AQ_pop(queue);
			UFroot[vtx] = vtx;
			UFrank[vtx] = 0;
		}
		double time4 = (double)clock() / CLOCKS_PER_SEC;
		printf("%d\n", answer);
		double time5 = (double)clock() / CLOCKS_PER_SEC;
		printf("print spent [%f]\n", time5 - time4);
		time11 += time2 - time1;
		time22 += time3 - time2;
		time33 += time4 - time3;
		time44 += time5 - time4;
		
	}
	AQ_deleteQueue(queue);
	free(UFroot);
	//free(visited);
	free(selected);
	double time2 = (double)clock() / CLOCKS_PER_SEC;
	printf("spent t %.2f, %.2f, %.2f, %.2f\n", time11, time22, time33, time44);
	printf("total K %d\n", totalk);
}
void TXTMAKE(int seed, int N, int Q) {
	int* visited = calloc(N + 1, sizeof(int));
	if (visited == NULL)exit(1);
	srand(seed);
	FILE* f = fopen("i2.txt", "w");
	fprintf(f, "%d\n", N);
	for (int i = 0; i < N - 1; i++) {
		int a, b, temp;
		a = rand() % N + 1;
		do {
			b = rand() % N + 1;
		} while (a == b);
		if (a > b) {
			temp = a;
			a = b;
			b = temp;
		}
		fprintf(f, "%d %d\n", a, b);
	}
	fprintf(f, "%d\n", Q);
	for (int i = 0; i < Q; i++) {
		memset(visited, 0, (N + 1) * sizeof(int));
		//int K = rand() % N + 1;
		int K = rand() % 2 + 3;
		fprintf(f, "%d ", K);
		for (int j = 0; j < K; j++) {
			int x;
			do {
				x = rand() % N + 1;
			} while (visited[x]);
			visited[x] = 1;
			fprintf(f, "%d ", x);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}
int main() {
	{
		TXTMAKE(777, 250000, 250000);
		freopen("i2.txt", "r", stdin);
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