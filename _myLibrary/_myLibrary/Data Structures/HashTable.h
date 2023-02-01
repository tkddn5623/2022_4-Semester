#ifndef ELEMENT_TYPE
#define ELEMENT_TYPE
typedef int Element;
#endif

#ifndef HASHTABLE_H
typedef struct _HNode {
	Element item;
	struct _HNode* next;
} HNode;
typedef struct {
	int blocksize;
	int nodesize;
	int nodecount;
	HNode** tails;
	HNode* _edges;
} Hashtable;
#define HASHTABLE_H
#endif

