#include <stdio.h>
#include <stdlib.h>
#define _INFINITE (1000000000)
typedef struct {
	int id;
	int weight;
} Edge;
typedef struct _GNode {
	Edge edge;
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** tails;
} Graph;
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
void GR_addEdge(const Graph* pgraph, const int index_from, const Edge _edge) {
	GNode* newNode;
	newNode = malloc(sizeof(GNode));
	newNode->edge = _edge;
	newNode->next = pgraph->tails[index_from]->next;
	pgraph->tails[index_from]->next = newNode;
	pgraph->tails[index_from] = newNode;
}
int solveBellmanFord(Graph* pgraph, const int vtxSize, const int edgeSize, const int vtxStart) {

}
int main() {
	int* distances;
	int V, E;
	Graph* graph;
	scanf("%d%d", &V, &E);
	graph = GR_newGraph(V + 1);
	for (int i = 0; i < E; i++) {
		int from;
		Edge edge;
		scanf("%d%d%d", &from, &edge.id, &edge.weight);
		GR_addEdge(graph, from, edge);
	}
	distances = solveBellmanFord(graph, V, E, 1);
	for (int i = 1; i <= V; i++) {
		if (distances[i] == _INFINITE) printf("INF\n");
		else printf("%d\n", distances[i]);
	}
	GR_deleteGraph(graph);
}