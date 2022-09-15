#ifndef UNION_FIND_H
#define UNION_FIND_H
int* UF_init(const int maxsize);
int UF_find(int* roots, const int key);
void UF_union(int* roots, int A, int B);
#endif