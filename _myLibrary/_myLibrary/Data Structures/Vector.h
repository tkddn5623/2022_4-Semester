#ifndef ELEMENT_TYPE
#define ELEMENT_TYPE
typedef int Element;
#endif

#ifndef VECTOR_H
#define VECTOR_H
typedef int Element;
typedef struct {
	Element* items;
	int capacity;
} Vector;
Vector* VT_newVector(const int firstSize);
void VT_deleteVector(Vector* pvector);
void VT_insert(Vector* pvector, const int index, const Element item);
Element VT_read(const Vector* pvector, const int index);
#endif



/*
* 2022.9.14 Wed
*/