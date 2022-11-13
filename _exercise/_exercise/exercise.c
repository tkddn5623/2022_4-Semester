#include <stdio.h>
#include <stdlib.h>
typedef long long Element;
typedef struct {
	Element item;
	int priority;
} HNode;
typedef struct {
	HNode* nodes;
	int size;
} ArrayHeap;
ArrayHeap * AH_newHeap(const int max) {
	ArrayHeap* pheap;
	if ((pheap = malloc(sizeof(ArrayHeap))) == NULL) exit(1);
	if ((pheap->nodes = calloc(max + 1, sizeof(HNode))) == NULL) exit(1);
	pheap->size = 0;
	return pheap;
}
void AH_deleteHeap(ArrayHeap* pheap) {
	if (pheap == NULL || pheap->nodes == NULL) exit(1);
	free(pheap->nodes);
	free(pheap);
}
void AH_flush(ArrayHeap* pheap) {
	pheap->size = 0;
}
int AH_size(const ArrayHeap* pheap) {
	return pheap->size;
}
void AH_push(ArrayHeap* pheap, const Element item, const int priority) {
	HNode newNode;
	int index = pheap->size + 1;
	while (index > 1) {
		int parentIndex = index / 2;
		if (priority < pheap->nodes[parentIndex].priority) {
			pheap->nodes[index] = pheap->nodes[parentIndex];
			index = parentIndex;
		}
		else break;
	}
	newNode.item = item;
	newNode.priority = priority;
	pheap->nodes[index] = newNode;
	pheap->size++;
}
Element AH_pop(ArrayHeap* pheap) {
	const int size = pheap->size;
	const Element topitem = pheap->nodes[1].item;
	const HNode last = pheap->nodes[size];
	int left, pickedChild, parentIndex = 1;
	while ((left = parentIndex * 2) <= size) {
		if (left == size) pickedChild = left;
		else if (pheap->nodes[left].priority < pheap->nodes[left + 1].priority) pickedChild = left;
		else pickedChild = left + 1;

		if (last.priority > pheap->nodes[pickedChild].priority) {
			pheap->nodes[parentIndex] = pheap->nodes[pickedChild];
			parentIndex = pickedChild;
		}
		else break;
	}
	pheap->nodes[parentIndex] = last;
	pheap->size--;
	return topitem;
}
long long getAnswer(ArrayHeap* heap) {
	long long a, sum = 0;
	while (AH_size(heap) > 1) {
		a = AH_pop(heap);
		a += AH_pop(heap);
		sum += a;
		AH_push(heap, a, a);
	}
	return sum;
}
int main() {
	{
		freopen("i.txt", "r", stdin);
	}
	int T, K, temp;
	ArrayHeap* heap = AH_newHeap(1000000);
	scanf("%d", &T);
	for (int t = 0; t < T; t++) {
		scanf("%d", &K);
		AH_flush(heap);
		for (int i = 0; i < K; i++) {
			scanf("%d", &temp);
			AH_push(heap, temp, temp);
		}
		printf("%lld\n", getAnswer(heap));
	}
}