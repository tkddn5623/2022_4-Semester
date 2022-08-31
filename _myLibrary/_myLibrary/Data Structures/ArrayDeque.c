#include <stdlib.h>
#include "ArrayDeque.h"
/*
typedef int Element;
typedef struct {
	Element* items;
	int capacity;
	int front;
	int rear;
} ArrayDeque;
*/
ArrayDeque* AD_newDeque(const int max) {
	ArrayDeque* pdeque;
	if ((pdeque = malloc(sizeof(ArrayDeque))) == NULL) exit(1);
	if ((pdeque->items = calloc(max + 1, sizeof(Element))) == NULL) exit(1);
	pdeque->capacity = max + 1;
	pdeque->front = 0;
	pdeque->rear = 0;
	return pdeque;
}
void AD_deleteDeque(ArrayDeque* pdeque) {
	if (pdeque == NULL || pdeque->items == NULL) exit(1);
	free(pdeque->items);
	free(pdeque);
}
int AD_isFull(const ArrayDeque* pdeque) {
	return pdeque->front == (pdeque->rear + 1) % pdeque->capacity;
}
int AD_isEmpty(const ArrayDeque* pdeque) {
	return pdeque->front == pdeque->rear;
}
int AD_size(const ArrayDeque* pdeque) {
	const int subtract = pdeque->rear - pdeque->front;
	if (subtract >= 0) return subtract;
	else return pdeque->capacity + subtract;
}
Element AD_front(const ArrayDeque* pdeque) {
	return pdeque->items[pdeque->front];
}
Element AD_back(const ArrayDeque* pdeque) {
	const int capacity = pdeque->capacity;
	return pdeque->items[(pdeque->rear + capacity - 1) % capacity];
}
void AD_pushFront(ArrayDeque* pdeque, const Element item) {
	const int capacity = pdeque->capacity, behind = (pdeque->front + capacity - 1) % capacity;
	pdeque->items[behind] = item;
	pdeque->front = behind;
}
void AD_pushBack(ArrayDeque* pdeque, const Element item) {
	const int rear = pdeque->rear;
	pdeque->items[rear] = item;
	pdeque->rear = (rear + 1) % pdeque->capacity;
}
Element AD_popFront(ArrayDeque* pdeque) {
	const int front = pdeque->front;
	pdeque->front = (front + 1) % pdeque->capacity;
	return pdeque->items[front];
}
Element AD_popBack(ArrayDeque* pdeque) {
	const int capacity = pdeque->capacity;
	const int behind = (pdeque->rear + capacity - 1) % capacity;
	pdeque->rear = behind;
	return pdeque->items[behind];
}