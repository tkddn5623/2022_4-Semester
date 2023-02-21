#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H
void SG_construct(long long _Dest[], const long long items[], const int item_len);
long long SG_get(const long long segtree[], int from, int to);
void SG_update(long long segtree[], int index, const long long value);
#endif