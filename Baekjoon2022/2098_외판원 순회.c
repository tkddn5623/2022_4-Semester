#include <stdio.h>
#include <stdlib.h>
typedef struct _GNode {
	int id;
	int weight;
	struct _GNode* next;
} GNode;
typedef struct {
	int vtxsize;
	int edgesize;
	int edgecount;
	GNode** tails;
	GNode* _edges;
} Graph;
Graph* GR_newGraph(const int vtxsize, const int edgesize) {
	Graph* pgraph;
	if ((pgraph = malloc(sizeof(Graph))) == NULL) exit(1);
	if ((pgraph->tails = calloc(vtxsize, sizeof(GNode*))) == NULL) exit(1);
	if ((pgraph->_edges = calloc(edgesize + vtxsize, sizeof(GNode))) == NULL) exit(1);
	pgraph->vtxsize = vtxsize;
	pgraph->edgesize = edgesize + vtxsize;
	pgraph->edgecount = vtxsize;
	for (int i = 0; i < vtxsize; i++) {
		pgraph->tails[i] = &pgraph->_edges[i];
		pgraph->tails[i]->next = pgraph->tails[i];
	}
	return pgraph;
}
void GR_deleteGraph(Graph* graph) {
	free(graph->_edges);
	free(graph->tails);
	free(graph);
}
void GR_addEdge(Graph* graph, const int from, const int to, const int weight) {
	graph->_edges[graph->edgecount].id = to;
	graph->_edges[graph->edgecount].weight = weight;
	graph->_edges[graph->edgecount].next = graph->tails[from]->next;
	graph->tails[from]->next = &graph->_edges[graph->edgecount];
	graph->tails[from] = &graph->_edges[graph->edgecount++];
}
int travelingSalesman(Graph* graph) {
	const int vtxsize = graph->vtxsize;
}
int main() {
	{freopen("i.txt", "r", stdin); }
	Graph* graph;
	int V;
	scanf("%d", &V);
	graph = GR_newGraph(V, V * V);
	for (int i = 0; i < V; i++) for (int j = 0; j < V; j++) {
		int weight;
		scanf("%d", &weight);
		if (weight > 0 && i != j) {
			GR_addEdge(graph, i, j, weight);
		}
	}
	printf("%d\n", travelingSalesman(graph));
	GR_deleteGraph(graph);
}