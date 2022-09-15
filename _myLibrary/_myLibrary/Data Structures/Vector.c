#include <stdlib.h>
#include "Vector.h";
/*
typedef int Element;
typedef struct {
	Element* items;
	int capacity;
} Vector;
*/
Vector* VT_newVector(const int firstSize) {
	if (firstSize <= 0) exit(1);
	Vector* pvector;
	if ((pvector = malloc(sizeof(Vector))) == NULL) exit(1);
	pvector->items = calloc(firstSize, sizeof(Element));
	pvector->capacity = firstSize;
	return pvector;
}
void VT_deleteVector(Vector* pvector) {
	if (pvector == NULL || pvector->items == NULL) exit(1);
	free(pvector->items);
	free(pvector);
}
void VT_insert(Vector* pvector, const int index, const Element item) {
	while (pvector->capacity <= index) {
		pvector->capacity *= 3;
		if ((pvector->items = realloc(pvector->items, pvector->capacity * sizeof(Element))) == NULL) exit(1);
	}
	pvector->items[index] = item;
}
Element VT_read(const Vector* pvector, const int index) {
	return pvector->items[index];
}