#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define _INFINITE (200000000)
typedef struct {
	int id;
	int weight;
} Edge;
typedef struct {
	Edge item;
	int priority;
} HNode;
typedef struct {
	HNode* nodes;
	int capacity;
	int size;
} ArrayHeap;
typedef struct _GNode {
	Edge edge;
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** tails;
} Graph;
ArrayHeap* AH_newHeap(const int max) {
	ArrayHeap* pheap;
	if ((pheap = malloc(sizeof(ArrayHeap))) == NULL) exit(1);
	if ((pheap->nodes = calloc(max + 1, sizeof(HNode))) == NULL) exit(1);
	pheap->capacity = max + 1;
	pheap->size = 0;
	return pheap;
}
void AH_deleteHeap(ArrayHeap* pheap) {
	if (pheap == NULL || pheap->nodes == NULL) exit(1);
	free(pheap->nodes);
	free(pheap);
}
int AH_isEmpty(const ArrayHeap* pheap) {
	return pheap->size == 0;
}
int AH_isFull(const ArrayHeap* pheap) {
	return pheap->size == pheap->capacity - 1;
}
void AH_push(ArrayHeap* pheap, const Edge item, const int priority) {
	HNode newNode;
	int index = pheap->size + 1;
	while (index > 1) {
		int parentIndex = index / 2;
		if (priority < pheap->nodes[parentIndex].priority) {
			pheap->nodes[index] = pheap->nodes[parentIndex];
			index = parentIndex;
		}
		else break;
	}
	newNode.item = item;
	newNode.priority = priority;
	pheap->nodes[index] = newNode;
	pheap->size++;
}
Edge AH_pop(ArrayHeap* pheap) {
	const int size = pheap->size;
	const Edge topitem = pheap->nodes[1].item;
	const HNode last = pheap->nodes[size];
	int left, pickedChild, parentIndex = 1;
	while ((left = parentIndex * 2) <= size) {
		if (left == size) pickedChild = left;
		else if (pheap->nodes[left].priority < pheap->nodes[left + 1].priority) pickedChild = left;
		else pickedChild = left + 1;

		if (last.priority > pheap->nodes[pickedChild].priority) {
			pheap->nodes[parentIndex] = pheap->nodes[pickedChild];
			parentIndex = pickedChild;
		}
		else break;
	}
	pheap->nodes[parentIndex] = last;
	pheap->size--;
	return topitem;
}
Graph* GR_newGraph(const int size) {
	Graph* pgraph;
	if ((pgraph = malloc(sizeof(Graph))) == NULL) exit(1);
	if ((pgraph->tails = calloc(size, sizeof(GNode*))) == NULL) exit(1);
	pgraph->size = size;
	for (int i = 0; i < size; i++) {
		pgraph->tails[i] = malloc(sizeof(GNode));
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
void GR_addEdge(const Graph* pgraph, const int index1, const int index2, const int weight) {
	GNode* newNode1, * newNode2;
	newNode1 = malloc(sizeof(GNode));
	newNode2 = malloc(sizeof(GNode));
	newNode1->edge.id = index1;
	newNode1->edge.weight = weight;
	newNode2->edge.id = index2;
	newNode2->edge.weight = weight;
	newNode1->next = pgraph->tails[index2]->next;
	pgraph->tails[index2]->next = newNode1;
	pgraph->tails[index2] = newNode1;
	newNode2->next = pgraph->tails[index1]->next;
	pgraph->tails[index1]->next = newNode2;
	pgraph->tails[index1] = newNode2;

}
int* solveDijkstra(const Graph* graph, const int vtxSize, const int edgeSize, const int vtxStart) {
	int* distances = calloc(vtxSize + 1, sizeof(int));
	ArrayHeap* heap = AH_newHeap(edgeSize);
	for (int i = 1; i <= vtxSize; i++) {
		distances[i] = _INFINITE;
	}
	distances[vtxStart] = 0;
	AH_push(heap, (Edge) { vtxStart, 0 }, 0);
	while (!AH_isEmpty(heap)) {
		Edge edge = AH_pop(heap);
		if (edge.weight > distances[edge.id]) continue;
		GNode* gn_head = graph->tails[edge.id]->next;
		GNode* gn_cur = gn_head->next;
		while (gn_cur != gn_head) {
			Edge destEdge = gn_cur->edge;
			destEdge.weight += edge.weight;
			if (destEdge.weight < distances[destEdge.id]) {
				if (AH_isFull(heap)) exit(1);
				AH_push(heap, destEdge, destEdge.weight);
				distances[destEdge.id] = destEdge.weight;
			}
			gn_cur = gn_cur->next;
		}
	}
	AH_deleteHeap(heap);
	return distances;
}
int main() {
	{
		freopen("i.txt", "r", stdin);
	}
	int** distances = calloc(3, sizeof(int*));
	int dist1, dist2;
	int V, E, checkPoint1, checkPoint2;
	Graph* graph;

	scanf("%d%d", &V, &E);
	graph = GR_newGraph(V + 1);
	for (int i = 0; i < E; i++) {
		int a, b, weight;
		scanf("%d%d%d", &a, &b, &weight);
		GR_addEdge(graph, a, b, weight);
	}
	scanf("%d%d", &checkPoint1, &checkPoint2);

	distances[0] = solveDijkstra(graph, V, E, 1);
	distances[1] = solveDijkstra(graph, V, E, checkPoint1);
	distances[2] = solveDijkstra(graph, V, E, checkPoint2);
	dist1 = distances[0][checkPoint1] + distances[1][checkPoint2] + distances[2][V];
	dist2 = distances[0][checkPoint2] + distances[2][checkPoint1] + distances[1][V];
	//printf("dist1 : %d + %d\ndist2 : %d + %d\n", distances[0][checkPoint1], distances[1][V], distances[0][checkPoint2], distances[2][V]);

	if (dist1 < _INFINITE && dist1 <= dist2) printf("%d\n", dist1);
	else if (dist2 < _INFINITE) printf("%d\n", dist2);
	else printf("-1\n");

	GR_deleteGraph(graph);
	free(distances[0]);
	free(distances[1]);
	free(distances[2]);
	free(distances);
}