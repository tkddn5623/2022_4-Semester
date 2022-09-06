#ifndef ELEMENT_TYPE
#define ELEMENT_TYPE
typedef int Element;
#endif

#ifndef ARRAY_HEAP_H
#define ARRAY_HEAP_H
typedef struct {
	Element item;
	int priority;
} HNode;
typedef struct {
	HNode* nodes;
	int capacity;
	int size;
} ArrayHeap;
ArrayHeap* AH_newHeap(const int max);
void AH_deleteHeap(ArrayHeap* pheap);
int AH_isEmpty(const ArrayHeap* pheap);
int AH_isFull(const ArrayHeap* pheap);
void AH_push(ArrayHeap* pheap, const Element item, const int priority);
Element AH_pop(ArrayHeap* pheap);
void AH_heapSort(Element* Base, const int NumOfElement);
#endif