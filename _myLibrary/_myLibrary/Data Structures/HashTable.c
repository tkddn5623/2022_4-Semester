#include <stdlib.h>
#include "Hashtable.h"
#define HASHBLOCKSIZE (100003)
/*
typedef int Element;
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
*/
Hashtable* HT_newTable(const int blocksize, const int nodesize) {
	Hashtable* table;
	if (!(table = malloc(sizeof(Hashtable)))) exit(1);
	if (!(table->tails = calloc(blocksize, sizeof(HNode*)))) exit(1);
	if (!(table->_edges = calloc(nodesize + blocksize, sizeof(HNode)))) exit(1);
	table->blocksize = blocksize;
	table->nodesize = nodesize + blocksize;
	table->nodecount = blocksize;
	for (int i = 0; i < blocksize; i++) {
		table->tails[i] = &table->_edges[i];
		table->tails[i]->next = table->tails[i];
	}
	return table;
}
void HT_deleteTable(Hashtable* table) {
	free(table->_edges);
	free(table->tails);
	free(table);
}
void HT_push(Hashtable* table, const int index, const Element item) {
	table->_edges[table->nodecount].item = item;
	table->_edges[table->nodecount].next = table->tails[index]->next;
	table->tails[index]->next = &table->_edges[table->nodecount];
	table->tails[index] = &table->_edges[table->nodecount++];
}
int HT_search(Hashtable* table, const int index, const Element item) {
	HNode* const head = table->tails[index]->next;
	for (HNode* cur = head->next; cur != head; cur = cur->next) {
		if (cur->item == item) return 1;
	}
	return 0;
}
int hashing(const char str[]) {
	unsigned int h = 0;
	char ch;
	while (ch = (*str++)) {
		h = (h + ch) * 5381u + ch;
	}
	return (int)(h % HASHBLOCKSIZE);
}


/*
* 2023.2.1 Hashtable
* 2023.2.1 And Simple STATIC hashing function.
*/