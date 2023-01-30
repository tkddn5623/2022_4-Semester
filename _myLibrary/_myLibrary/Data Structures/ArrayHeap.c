#include <stdlib.h>
#include "ArrayHeap.h"
/*
typedef int Element;
typedef struct {
	Element item;
	int priority;
} HNode;
typedef struct {
	HNode* nodes;
	int capacity;
	int size;
} ArrayHeap;
*/
ArrayHeap* AH_newHeap(const int max) {
	ArrayHeap* pheap;
	if ((pheap = malloc(sizeof(ArrayHeap))) == NULL) exit(1);
	if ((pheap->nodes = calloc(max + 1, sizeof(HNode))) == NULL) exit(1);
	pheap->capacity = max + 1;
	pheap->size = 0;
	return pheap;
}
void AH_deleteHeap(ArrayHeap* pheap) {
	free(pheap->nodes);
	free(pheap);
}
int AH_isEmpty(const ArrayHeap* pheap) {
	return pheap->size == 0;
}
int AH_isFull(const ArrayHeap* pheap) {
	return pheap->size == pheap->capacity - 1;
}
void AH_push(ArrayHeap* pheap, const Element item, const int priority) {
	HNode newNode;
	int index = pheap->size + 1;
	while (index > 1) {
		int parentIndex = index / 2;
		if (priority < pheap->nodes[parentIndex].priority) { 
			//Minheap
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
		//Minheap
		else pickedChild = left + 1;
		
		if (last.priority > pheap->nodes[pickedChild].priority) { 
			//Minheap
			pheap->nodes[parentIndex] = pheap->nodes[pickedChild];
			parentIndex = pickedChild;
		}
		else break;
	}
	pheap->nodes[parentIndex] = last;
	pheap->size--;
	return topitem;
}
void AH_heapSort(Element* Base, const int NumOfElement) {
	ArrayHeap* heap = AH_newHeap(NumOfElement);
	for (int i = 0; i < NumOfElement; i++) 
		AH_push(heap, Base[i], Base[i]);
	for (int i = 0; i < NumOfElement; i++)
		Base[i] = AH_pop(heap);
	AH_deleteHeap(heap);
}



/*
* 2022.7.27 Wed
*/