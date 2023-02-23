/**************************************************************************
* BOJ 6549, Leetcode 84.                                                  *
* Largest Rectangle in a Histogram.                                       *
*                                                                         *
* PLAN A: Segment Tree.                                                   *
* PLAN B: Divide n Conquer.                                               *
* PLAN C: Stack.                                                          *
*                                                                         *
* This code use 'Segment Tree'.                                           *
* Reference: sgc109.github.io/2021/03/18/largest-rectangle-in-histogram/  *
***************************************************************************/
#include <stdio.h>
#include <string.h>
#define MAXSIZE (100000)
#define MAXTREE (1 << 18)
inline int _max(const int _A, const int _B) {
	return _A > _B ? _A : _B;
}
int BinarySearch(const int arr[], int size, const int key) {
	int pos = -1, left = 0, mid;
	size = size - 1;
	while (left <= size) {
		mid = left + (size - left) / 2;
		if (arr[mid] == key) {
			pos = mid;
			size = mid - 1;
		}
		else if (arr[mid] < key) left = mid + 1;
		else size = mid - 1;
	}
	return pos;
}
void _MergeSort_impl(int* list, int* sorted, const int left, const int right) {
	if (left >= right) return;
	const int mid = left + (right - left) / 2;
	_MergeSort_impl(list, sorted, left, mid);
	_MergeSort_impl(list, sorted, mid + 1, right);
	int idx, first, second;
	idx = left, first = left, second = mid + 1;
	while (first <= mid && second <= right) {
		if (list[first] <= list[second]) sorted[idx++] = list[first++];
		else sorted[idx++] = list[second++];
	}
	while (first <= mid)    sorted[idx++] = list[first++];
	while (second <= right) sorted[idx++] = list[second++];
	for (int i = left; i <= right; i++) list[i] = sorted[i];
}
int Coordinate_compress(int* _Dest, const int* _Src, const int size) {
	static int sorted[MAXSIZE + 2], list[MAXSIZE + 2];
	int last, len = 0;
	memcpy(list, _Src, sizeof(int) * size);
	_MergeSort_impl(list, sorted, 0, size - 1);
	_Dest[len++] = last = list[0];
	for (int i = 1; i < size; i++) {
		if (last == list[i]) continue;
		_Dest[len++] = last = list[i];
	}
	return len;
}
void SG_init(int _Dest[], const int item_len) {
	int leafs = 1;
	memset(_Dest, 0, sizeof(int) * MAXTREE);
	while (leafs < item_len) { leafs <<= 1; }
	_Dest[0] = leafs;
}
int SG_get(const int segtree[], int from, int to) {
	int ret = 0;
	from += segtree[0]; to += segtree[0];
	while (from <= to) {
		if (from % 2 == 1) ret = _max(ret, segtree[from++]);
		if (to % 2 == 0) ret = _max(ret, segtree[to--]);
		from /= 2; to /= 2;
	}
	return ret;
}
void SG_update(int segtree[], int index, const int value) {
	index += segtree[0];
	segtree[index] = value;
	while ((index /= 2) >= 1) {
		segtree[index] = _max(segtree[index * 2], segtree[index * 2 + 1]);
	}
}
long long largest_Rectangle_Histogram(const int arr[], const int len) {
	static int segtree[MAXTREE], heights[MAXSIZE + 1];
	static int leftmost[MAXSIZE + 2], rightmost[MAXSIZE + 2];
	int heights_len = Coordinate_compress(heights, arr, len + 1);
	long long area_max = 0;
	SG_init(segtree, len + 1);
	for (int i = 1; i <= len; i++) {
		int hidx = BinarySearch(heights, heights_len, arr[i]);
		leftmost[i] = SG_get(segtree, 0, hidx - 1);
		SG_update(segtree, hidx, i);
	}
	SG_init(segtree, len + 1);
	for (int i = 1; i <= len; i++) {
		int hidx = BinarySearch(heights, heights_len, arr[len + 1 - i]);
		rightmost[len + 1 - i] = len + 1 - SG_get(segtree, 0, hidx - 1);
		SG_update(segtree, hidx, i);
	}
	for (int i = 1; i <= len; i++) {
		long long area_cur = (long long)arr[i] * (rightmost[i] - leftmost[i] - 1);
		if (area_max < area_cur) area_max = area_cur;
	}
	return area_max;
}
int main() {
	static int arr[MAXSIZE + 2];
	int N;
	while (1) {
		scanf("%d", &N);
		if (N <= 0) break;
		for (int i = 1; i <= N; i++) { scanf("%d", &arr[i]); }
		arr[N + 1] = 0;
		printf("%lld\n", largest_Rectangle_Histogram(arr, N));
	}
}