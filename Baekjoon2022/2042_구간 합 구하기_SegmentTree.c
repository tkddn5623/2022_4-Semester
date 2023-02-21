#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE (1000000)
#define MAXTREE (1 << 21)
void SG_construct(long long _Dest[], const long long items[], const int item_len) {
	int leafs = 1;
	while (leafs < item_len) { leafs <<= 1; }
	for (int i = 0; i < item_len; i++) {
		_Dest[i + leafs] = items[i];
	}
	for (int i = leafs - 1; i >= 1; i--) {
		_Dest[i] = _Dest[i * 2] + _Dest[i * 2 + 1];
	}
	_Dest[0] = leafs;
}
long long SG_get(const long long segtree[], int from, int to) {
	long long ret = 0;
	from += segtree[0]; to += segtree[0];
	while (from <= to) {
		if (from % 2 == 1) ret += segtree[from++];
		if (to % 2 == 0) ret += segtree[to--];
		from /= 2; to /= 2;
	}
	return ret;
}
void SG_update(long long segtree[], int index, const long long value) {
	index += segtree[0];
	segtree[index] = value;
	while ((index /= 2) >= 1) {
		segtree[index] = segtree[index * 2] + segtree[index * 2 + 1];
	}
}
int main() {
	static long long arr[MAXSIZE];
	static long long segtree[MAXTREE];
	int N, M, K;

	scanf("%d%d%d", &N, &M, &K);
	for (int i = 0; i < N; i++) { scanf("%lld", &arr[i]); }
	SG_construct(segtree, arr, N);

	for (int i = 0; i < M + K; i++) {
		int type;
		long long a, b;
		scanf("%d%lld%lld", &type, &a, &b);
		switch (type) {
		case 1:
			SG_update(segtree, a - 1, b);
			break;
		case 2:
			printf("%lld\n", SG_get(segtree, a - 1, b - 1));
			break;
		}
	}

}