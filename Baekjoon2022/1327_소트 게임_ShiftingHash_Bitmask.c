#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASH_MSB_BITS (16)
#define HASHBLOCKSIZE (1 << HASH_MSB_BITS)
#define SHIFTBITS (4)
#define MAXN (8)
typedef int Element;
typedef struct {
	Element* items;
	int capacity;
	int front;
	int rear;
} ArrayQueue;
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
ArrayQueue* AQ_newQueue(const int max) {
	ArrayQueue* pqueue;
	if ((pqueue = malloc(sizeof(ArrayQueue))) == NULL) exit(1);
	if ((pqueue->items = calloc(max + 1, sizeof(Element))) == NULL) exit(1);
	pqueue->capacity = max + 1;
	pqueue->front = 0;
	pqueue->rear = 0;
	return pqueue;
}
void AQ_deleteQueue(ArrayQueue* pqueue) {
	free(pqueue->items);
	free(pqueue);
}
int AQ_isEmpty(const ArrayQueue* pqueue) {
	return pqueue->front == pqueue->rear;
}
int AQ_size(const ArrayQueue* pqueue) {
	const int subtract = pqueue->rear - pqueue->front;
	if (subtract >= 0) return subtract;
	else return pqueue->capacity + subtract;
}
Element AQ_front(const ArrayQueue* pqueue) {
	return pqueue->items[pqueue->front];
}
void AQ_push(ArrayQueue* pqueue, const Element item) {
	const int rear = pqueue->rear;
	pqueue->items[rear] = item;
	pqueue->rear = (rear + 1) % pqueue->capacity;
}
Element AQ_pop(ArrayQueue* pqueue) {
	const int front = pqueue->front;
	pqueue->front = (front + 1) % pqueue->capacity;
	return pqueue->items[front];
}
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
	HNode* const head = &table->_edges[index];
	for (HNode* cur = head->next; cur != head; cur = cur->next) {
		if (cur->item == item) return 1;
	}
	return 0;
}
int simple_hash(unsigned int k) {
	return (k * 2654435769u) >> (32 - HASH_MSB_BITS);
}
int solve1327(const char str[], const int N, const int K) {
	ArrayQueue* queue = AQ_newQueue(HASHBLOCKSIZE);
	Hashtable* visited_map = HT_newTable(HASHBLOCKSIZE, HASHBLOCKSIZE);
	char buffer[MAXN + 2] = { 0 };
	int prevqsize, answer = -1, count = 0;
	unsigned int bitmask, sorted_bitmask;

	sorted_bitmask = 0; //Ascending bitmask
	for (int i = 0; i < N; i++) {
		sorted_bitmask |= (i + 1) << (i * SHIFTBITS);
	}

	bitmask = 0; //String to bitmask
	for (int i = 0; i < N; i++) {
		bitmask |= (str[i] - '0') << (i * SHIFTBITS);
	}
	HT_push(visited_map, simple_hash(bitmask), bitmask);
	AQ_push(queue, bitmask);
	prevqsize = AQ_size(queue);

	while (!AQ_isEmpty(queue)) { //Breadth First Search
		if (sorted_bitmask == (bitmask = AQ_pop(queue))) {
			answer = count;
			break;
		}
		for (int i = 0; i < N; i++) { //Bitmask to string
			buffer[i] = ((bitmask >> (i * SHIFTBITS)) & ((1 << SHIFTBITS) - 1)) + '0';
		}
		for (int i = 0; i < N + 1 - K; i++) {
			bitmask = 0;
			for (int j = 0; j < N; j++) {
				if (i <= j && j < i + K) {
					bitmask |= (buffer[2 * i + K - 1 - j] - '0') << (j * SHIFTBITS);
				}
				else {
					bitmask |= (buffer[j] - '0') << (j * SHIFTBITS);
				}
			}
			if (!HT_search(visited_map, simple_hash(bitmask), bitmask)) {
				HT_push(visited_map, simple_hash(bitmask), bitmask);
				AQ_push(queue, bitmask);
			}
		}
		if (--prevqsize == 0) {
			prevqsize = AQ_size(queue);
			count++;
		}
	}
	AQ_deleteQueue(queue);
	HT_deleteTable(visited_map);
	return answer;
}
int main() {
	char str[MAXN + 2] = { 0 };
	int N, K;
	char ch;
	scanf("%d%d", &N, &K);
	for (int i = 0; i < N;) {
		ch = getchar();
		if ('0' <= ch && ch <= '9') {
			str[i++] = ch;
		}
	}
	printf("%d\n", solve1327(str, N, K));
}