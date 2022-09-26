#ifndef GRAPH_H
#define GRAPH_H
typedef struct _GNode {
	int id;
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** tails;
} Graph;
Graph* GR_newGraph(const int size);
void GR_deleteGraph(Graph* pgraph);
void GR_addEdge(const Graph* pgraph, const int index1, const int index2);
#endif