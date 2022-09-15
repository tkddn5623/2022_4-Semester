#ifndef GRAPH_H
#define GRAPH_H
typedef struct _GNode {
	int id;
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** heads;
	GNode** tails;
} Graph;
#endif