#ifndef ELEMENT_TYPE
#define ELEMENT_TYPE
typedef int Element;
#endif

#ifndef MERGE_SORT_H
void _MergeSort_impl(Element* const list, Element* const sorted, const int left, const int right);
Element* MergeSort(Element* list, const int size);
#define MERGE_SORT_H
#endif
