#include <stdio.h>

#define MXN 20000
#define MXM 300000

typedef struct {
	int dist, to;
	int next;
} Edge;
Edge E[MXM];
int Ecnt;

int Adj[MXN];

void init(int n) {
	for (int i = 0; i < n; i++) {
		Adj[i] = -1;
	}
	Ecnt = 0;
}

void pushEdge(int a, int b, int w) {
	int eid = Ecnt++;
	E[eid].to = b;
	E[eid].dist = w;
	E[eid].next = Adj[a];
	Adj[a] = eid;
}

typedef struct {
	int to;
	int dist;
} QEntry;
QEntry PQ[MXM * 2];
int Qlen;

#define SWAP(t, x, y)	{ t=(x); (x)=(y); (y)=t; }

void inline swapQ(int a, int b) {
	int t;
	SWAP(t, PQ[a].to, PQ[b].to);
	SWAP(t, PQ[a].dist, PQ[b].dist);
}

void pushQ(int to, int dist) {
	int i = Qlen++;
	PQ[i].to = to;
	PQ[i].dist = dist;
	while (i > 0) {
		int p = (i - 1) / 2;
		if (PQ[p].dist > PQ[i].dist) {
			swapQ(p, i);
			i = p;
		}
		else
			break;
	}
}

QEntry* popQ() {
	QEntry* ret = NULL;
	if (Qlen > 0) {
		swapQ(0, --Qlen);
		ret = &PQ[Qlen];
		int i = 0;
		while (i * 2 + 1 < Qlen) {
			int l = i * 2 + 1, r = l + 1;
			if (r < Qlen && PQ[r].dist < PQ[l].dist && PQ[r].dist < PQ[i].dist) {
				swapQ(r, i); i = r;
			}
			else if (PQ[l].dist < PQ[i].dist) {
				swapQ(l, i); i = l;
			}
			else
				break;
		}
	}
	return ret;
}

#define INF (1<<30)

int D[MXN];
int Vdone[MXN];

void spf(int n, int s)
{
	for (int i = 0; i < n; i++) {
		D[i] = INF;
		Vdone[i] = 0;
	}
	D[s] = 0;
	Qlen = 0;
	pushQ(s, 0);
	QEntry *qe;
	while ((qe = popQ()) != NULL) {
		int i = qe->to;
		if (Vdone[i]) continue;
		Vdone[i] = 1;
		for (int eid = Adj[i]; eid >= 0; eid = E[eid].next) {
			int to = E[eid].to;
			if (D[to] > D[i] + E[eid].dist) {
				D[to] = D[i] + E[eid].dist;
				pushQ(to, D[to]);
			}
		}
	}
}
int main()
{
	int N, M, S; scanf("%d %d %d", &N, &M, &S);
	init(N);
	for (int i = 0; i < M; i++) {
		int A, B, C; scanf("%d %d %d", &A, &B, &C);
		pushEdge(A - 1, B - 1, C);
	}
	spf(N, S-1);
	for (int i = 0; i < N; i++) {
        if (D[i] == INF) printf("INF\n");
		else printf("%d\n", D[i]);
	}
    return 0;
}