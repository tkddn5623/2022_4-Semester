#include <stdlib.h>
#include "AdjacencyList.h"
/*
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
*/
Graph* GR_newGraph(const int vtxsize, const int edgesize) {
	Graph* graph;
	if ((graph = malloc(sizeof(Graph))) == NULL) exit(1);
	if ((graph->tails = calloc(vtxsize, sizeof(GNode*))) == NULL) exit(1);
	if ((graph->_edges = calloc(edgesize + vtxsize, sizeof(GNode))) == NULL) exit(1);
	graph->vtxsize = vtxsize;
	graph->edgesize = edgesize + vtxsize;
	graph->edgecount = vtxsize;
	for (int i = 0; i < vtxsize; i++) {
		graph->tails[i] = &graph->_edges[i];
		graph->tails[i]->next = graph->tails[i];
	}
	return graph;
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
/*
int _travelingSalesman_impl(Graph* graph, int memo[][1 << MAXVTX], const int vtx, int visited_bit) {
	int dist_min;
	const int vtxsize = graph->vtxsize;
	GNode* const head = &graph->_edges[vtx];
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
*/

/*
* 2022.9.21 Wed
* 2023.1.6  Fri, improved speed.
* 2023.1.27 Fri, example changed to TSP.
*/

/*
GNode* const head = graph->tails[vtx]->next; //Slower
GNode* const head = &graph->_edges[vtx];     //Faster (in TSP, difference is small)
*/