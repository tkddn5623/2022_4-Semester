#ifndef UNION_FIND_H
#define UNION_FIND_H
int* UF_init(const int maxsize); //In gcc-O2, this function makes the code slower.
int UF_find(int* roots, const int key);
void UF_union(int* roots, int A, int B);
#endif