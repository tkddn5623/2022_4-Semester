/*
This code is just a 'practice without STL' for adjacency list, Don't study my dirty codes.
*/

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
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** tails;
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
Element AQ_back(const ArrayQueue* pqueue) {
	const int capacity = pqueue->capacity;
	return pqueue->items[(pqueue->rear + capacity - 1) % capacity];
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
void GR_addEdge(const Graph* pgraph, const int index_from, const int index_to) {
	GNode* newNode;
	newNode = malloc(sizeof(GNode));
	newNode->id = index_to;
	newNode->next = pgraph->tails[index_from]->next;
	pgraph->tails[index_from]->next = newNode;
	pgraph->tails[index_from] = newNode;
}
int GR_BFS(Graph* pgraph, const int startIdx, const int endIdx) {
	const int size = pgraph->size;
	int* visited = calloc(size, sizeof(int));
	int count = 2, flag = 0;
	int prevQueueSize;
	ArrayQueue* queue = AQ_newQueue(size);
	if (!visited[startIdx]) {
		AQ_push(queue, startIdx);
		visited[startIdx] = 1;
	}
	prevQueueSize = 1;
	while (!AQ_isEmpty(queue) && !flag) {
		int vtx = AQ_pop(queue);
		prevQueueSize--;
		GNode* _head = pgraph->tails[vtx]->next;
		GNode* cur = _head->next;
		while (cur != _head) {
			if (!visited[cur->id]) {
				if (cur->id == endIdx) {
					flag = 1;
					break;
				}
				AQ_push(queue, cur->id);
				visited[cur->id] = 1;

			}
			cur = cur->next;
		}
		if (!flag && prevQueueSize == 0) {
			prevQueueSize = AQ_size(queue);
			count++;
		}
	}
	AQ_deleteQueue(queue);
	return count;
}
int main() {
	char buffer[120];
	int row, col, q;
	Graph* graph;
	scanf("%d%d", &row, &col);
	graph = GR_newGraph(row * col + 1);
	scanf("%s", buffer);
	if (buffer[0] == '1') {
		GR_addEdge(graph, 1 + 1, 1);
		GR_addEdge(graph, 1 + col, 1);
	}
	for (int _col = 2; _col < col; _col++) {
		if (buffer[_col - 1] == '1') {
			GR_addEdge(graph, _col + 1, _col);
			GR_addEdge(graph, _col - 1, _col);
			GR_addEdge(graph, _col + col, _col);
		}
	}
	if (buffer[col - 1] == '1') {
		GR_addEdge(graph, col - 1, col);
		GR_addEdge(graph, col + col, col);
	}
	for (int i = 1; i < row - 1; i++) {
		q = i * col;
		scanf("%s", buffer);
		if (buffer[0] == '1') {
			GR_addEdge(graph, q + 1 - col, q + 1);
			GR_addEdge(graph, q + 1 + 1, q + 1);
			GR_addEdge(graph, q + 1 + col, q + 1);
		}
		for (int _col = 2; _col < col; _col++) {
			if (buffer[_col - 1] == '1') {
				GR_addEdge(graph, q + _col - col, q + _col);
				GR_addEdge(graph, q + _col + 1, q + _col);
				GR_addEdge(graph, q + _col - 1, q + _col);
				GR_addEdge(graph, q + _col + col, q + _col);
			}
		}
		if (buffer[col - 1] == '1') {
			GR_addEdge(graph, q + col - col, q + col);
			GR_addEdge(graph, q + col - 1, q + col);
			GR_addEdge(graph, q + col + col, q + col);
		}
	}
	q = (row - 1) * col;
	scanf("%s", buffer);
	if (buffer[0] == '1') {
		GR_addEdge(graph, q + 1 - col, q + 1);
		GR_addEdge(graph, q + 1 + 1, q + 1);
	}
	for (int _col = 2; _col < col; _col++) {
		if (buffer[_col - 1] == '1') {
			GR_addEdge(graph, q + _col - col, q + _col);
			GR_addEdge(graph, q + _col + 1, q + _col);
			GR_addEdge(graph, q + _col - 1, q + _col);
		}
	}
	if (buffer[col - 1] == '1') {
		GR_addEdge(graph, q + col - col, q + col);
		GR_addEdge(graph, q + col - 1, q + col);
	}
	printf("%d\n", GR_BFS(graph, 1, row * col));
}
