#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN (200100)
#define POLYCOEF (47)
#define HASHSIZE (172507)
typedef const char* Element;
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
void HT_flush(Hashtable* table) {
	int i = table->nodecount = table->blocksize;
	while (i--) {
		table->tails[i] = &table->_edges[i];
		table->tails[i]->next = table->tails[i];
	}
}
void HT_push(Hashtable* table, const int index, Element item) {
	table->_edges[table->nodecount].item = item;
	table->_edges[table->nodecount].next = table->tails[index]->next;
	table->tails[index]->next = &table->_edges[table->nodecount];
	table->tails[index] = &table->_edges[table->nodecount++];
}
int HT_search(Hashtable* table, const int index, Element item, const int itemlen) {
	HNode* const head = table->tails[index]->next;
	for (HNode* cur = head->next; cur != head; cur = cur->next) {
		if (!strncmp(cur->item, item, itemlen)) return 1;
	}
	return 0;
}
int solve3033(const char text[], const int N) {
	Hashtable* table = HT_newTable(HASHSIZE, MAXLEN);
	int length, L = 1, R = N;
	int answer = 0;
	while (L <= R) {
		length = L + (R - L) / 2;
		int power = 1, hash = 0;

		for (int i = length - 1; i >= 0; i--) {
			hash += text[i] * power;
			hash %= HASHSIZE;
			power = (power * POLYCOEF) % HASHSIZE;
		}
		HT_push(table, hash, &text[0]);

		for (int i = 1; i <= N - length; i++) {
			hash = (hash * POLYCOEF + text[i + length - 1] - text[i - 1] * power) % HASHSIZE;
			if (hash < 0) hash = HASHSIZE + hash;

			if (!HT_search(table, hash, &text[i], length)) {
				HT_push(table, hash, &text[i]);
			}
			else {
				answer = length;
				L = length + 1;
				break;
			}
		}
		if (answer < length) R = length - 1;
		HT_flush(table);
	}
	HT_deleteTable(table);
	return answer;
}
int main() {
	int N;
	char* cp;
	static char text[MAXLEN + 2];
	scanf("%d ", &N);
	fgets(text, sizeof(text), stdin);
	if (cp = strchr(text, '\n')) *cp = '\0';
	printf("%d\n", solve3033(text, N));
}