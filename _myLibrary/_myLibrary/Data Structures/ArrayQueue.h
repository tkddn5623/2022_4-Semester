#ifndef ELEMENT_TYPE
typedef int Element;
#define ELEMENT_TYPE
#endif

#ifndef ARRAY_QUEUE_H
#define ARRAY_QUEUE_H
typedef struct {
	Element* items;
	int capacity;
	int front;
	int rear;
} ArrayQueue;
ArrayQueue* AQ_newQueue(const int max);
void AQ_deleteQueue(ArrayQueue* pqueue);
int AQ_isFull(const ArrayQueue* pqueue);
int AQ_isEmpty(const ArrayQueue* pqueue);
int AQ_size(const ArrayQueue* pqueue);
Element AQ_front(const ArrayQueue* pqueue);
Element AQ_back(const ArrayQueue* pqueue);
void AQ_push(ArrayQueue* pqueue, const Element item);
Element AQ_pop(ArrayQueue* pqueue);
#endif


/*
* 2022.7.22 Fri
*/