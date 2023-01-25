#ifndef GRAPH_H
#define GRAPH_H
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
Graph* GR_newGraph(const int vtxsize, const int edgesize);
void GR_deleteGraph(Graph * graph);
void GR_addEdge(Graph * graph, const int from, const int to, const int weight);
#endif