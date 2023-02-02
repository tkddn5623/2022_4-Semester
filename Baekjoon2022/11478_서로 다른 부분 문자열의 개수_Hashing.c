#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN (1000)
#define HASHBLOCKSIZE (100003)
typedef struct {
	int left;
	int right;
} pair_t;
typedef pair_t Element;
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
int HT_search(const Hashtable* table, const int index, const Element position, const char keystr[]) {
	HNode* const head = table->tails[index]->next;
	for (HNode* cur = head->next; cur != head; cur = cur->next) {
		const Element e = cur->item;
		if (e.right - e.left == position.right - position.left
			&& !strncmp(keystr + e.left, keystr + position.left, e.right - e.left + 1)) {
			return 1;
		}
	}
	return 0;
}
int hashing_n(const char str[], int N) {
	unsigned int h = 0;
	char ch;
	while (N-- && (ch = (*str++))) {
		h = (h + ch) * 5381u + ch;
	}
	return (int)(h % HASHBLOCKSIZE);
}
int num_of_substrings(const char str[]) {
	const int len = strlen(str);
	if (len <= 0) return 0;
	Hashtable* hashtable = HT_newTable(HASHBLOCKSIZE, (len * (len + 1)) / 2);
	int answer = 0;
	for (int i = 0; i < len; i++) {
		for (int j = i; j < len; j++) {
			int hashid = hashing_n(str + i, j - i + 1);
			if (!HT_search(hashtable, hashid, (pair_t) { i, j }, str)) {
				HT_push(hashtable, hashid, (pair_t) { i, j });
				answer++;
			}
		}
	}
	HT_deleteTable(hashtable);
	return answer;
}
int main() {
	char str[MAXLEN + 2];
	scanf("%s", str);
	printf("%d\n", num_of_substrings(str));
}