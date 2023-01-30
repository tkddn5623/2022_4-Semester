#include <stdlib.h>
#include "ArrayStack.h"
/*
typedef int Element;
typedef struct {
	Element* items;
	int capacity;
	int top;
} ArrayStack;
*/
ArrayStack* AS_newStack(const int max) {
	ArrayStack* pstack;
	if ((pstack = malloc(sizeof(ArrayStack))) == NULL) exit(1);
	if ((pstack->items = calloc(max, sizeof(Element))) == NULL) exit(1);
	pstack->capacity = max;
	pstack->top = -1;
	return pstack;
}
void AS_deleteStack(ArrayStack* pstack) {
	free(pstack->items);
	free(pstack);
}
int AS_isFull(const ArrayStack* pstack) {
	return pstack->top == pstack->capacity - 1;
}
int AS_isEmpty(const ArrayStack* pstack) {
	return pstack->top == -1;
}
int AS_size(const ArrayStack* pstack) {
	return pstack->top + 1;
}
Element AS_peek(const ArrayStack* pstack) {
	return pstack->items[pstack->top];
}
void AS_push(ArrayStack* pstack, const Element item) {
	pstack->items[++(pstack->top)] = item;
}
Element AS_pop(ArrayStack* pstack) {
	return pstack->items[(pstack->top)--];
}
