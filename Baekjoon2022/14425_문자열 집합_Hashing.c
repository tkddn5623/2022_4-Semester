#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXBUF (510)
#define HASHBLOCKSIZE (100003)
typedef char* Element;
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
		if (!strcmp(cur->item, item)) return 1;
	}
	return 0;
}
int hashing(const char str[]) {
	unsigned int h = 0;
	char ch;
	while (ch = (*str++)) {
		h = (h + ch) * 31u + ch;
		//h %= HASHBLOCKSIZE;
	}
	return (int)(h % HASHBLOCKSIZE);
}
int main() {
	char buffer[MAXBUF] = { 0 };
	Hashtable* hashtable;
	char** str;
	int N, M, answer = 0;
	scanf("%d%d", &N, &M);
	hashtable = HT_newTable(HASHBLOCKSIZE, N-1);
	if (!(str = calloc(N, sizeof(char*)))) exit(1);
	for (int i = 0; i < N; i++) {
		scanf("%s", buffer);
		if (!(str[i] = calloc(strlen(buffer) + 1, sizeof(char)))) exit(1);
		strcpy(str[i], buffer);
		HT_push(hashtable, hashing(str[i]), str[i]);
	}
	for (int i = 0; i < M; i++) {
		scanf("%s", buffer);
		answer += HT_search(hashtable, hashing(buffer), buffer);
	}
	printf("%d\n", answer);
	HT_deleteTable(hashtable);
	for (int i = 0; i < N; i++) { free(str[i]); }
	free(str);
}