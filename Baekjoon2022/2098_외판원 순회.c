#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF (1 << 24) //1.67e7
#define MAXVTX (16)
#define HOME (0)
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
int _travelingSalesman_impl(Graph* graph, int memo[][1 << MAXVTX], const int vtx, int visited_bit) {
	int dist_min;
	const int vtxsize = graph->vtxsize;
	GNode* const head = graph->tails[vtx]->next;
	//if (memo[vtx][visited_bit] != -1) return memo[vtx][visited_bit];
	if (visited_bit == (1 << vtxsize) - 1) {
		for (GNode* cur = head->next; cur != head; cur = cur->next) {
			if (cur->id == HOME) {
				return cur->weight;
			}
		}
		return INF;
	}
	dist_min = INF;
	for (GNode* cur = head->next; cur != head; cur = cur->next) {
		if (visited_bit & (1 << cur->id)) continue;
		const int mem = memo[cur->id][visited_bit | 1 << cur->id];
		int dist = cur->weight;
		if (mem == -1) dist += _travelingSalesman_impl(graph, memo, cur->id, (visited_bit | 1 << cur->id));
		else dist += mem;
		dist_min = dist < dist_min ? dist : dist_min;
	}
	memo[vtx][visited_bit] = dist_min;
	return dist_min;
}
int travelingSalesman(Graph* graph) {
	static int memo[MAXVTX][1 << MAXVTX];
	memset(memo, -1, sizeof(memo));
	return _travelingSalesman_impl(graph, memo, HOME, 1 << HOME);
}
int main() {
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
