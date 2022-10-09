#include <stdio.h>
#include <stdlib.h>
typedef int Element;
typedef struct {
	Element parent;
	int distance;
} Vertex;
typedef struct {
	Element* items;
	int capacity;
	int top;
} ArrayStack;
typedef struct _GNode {
	int id;
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** tails;
} Graph;
ArrayStack* AS_newStack(const int max) {
	ArrayStack* pstack;
	if ((pstack = malloc(sizeof(ArrayStack))) == NULL) exit(1);
	if ((pstack->items = calloc(max, sizeof(Element))) == NULL) exit(1);
	pstack->capacity = max;
	pstack->top = -1;
	return pstack;
}
void AS_deleteStack(ArrayStack* pstack) {
	if (pstack == NULL || pstack->items == NULL) exit(1);
	free(pstack->items);
	free(pstack);
}
int AS_isEmpty(const ArrayStack* pstack) {
	return pstack->top == -1;
}
int AS_size(const ArrayStack* pstack) {
	return pstack->top + 1;
}
Element AS_peek(const ArrayStack* pstack) {
	return pstack->items[pstack->top];
}
void AS_push(ArrayStack* pstack, const Element item) {
	pstack->items[++(pstack->top)] = item;
}
Element AS_pop(ArrayStack* pstack) {
	return pstack->items[(pstack->top)--];
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
int GR_DFS(Graph* pgraph, const int startIdx) {
	const int size = pgraph->size;
	int count = 0;
	int* visited = calloc(size, sizeof(int));
	ArrayQueue* queue = AQ_newQueue(size);
	//Start from here, fix this to stack.
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
				praph->tails[cur->id]->next->id = vtx; //If it is tree, this stores the 'parent' index.
				count++;
			}
			cur = cur->next;
		}
	}
	AQ_deleteQueue(queue);
	return count;
}
int main() {
	int V;
	Vertex* vertices;
	int* longestDistances;
	scanf("%d", &V);
	vertices = calloc(V + 1, sizeof(Vertex));
	longestDistances = calloc(V + 1, sizeof(int));
	for (int i = 1; i <= V; i++) {

	}


}