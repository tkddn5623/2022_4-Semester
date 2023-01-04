#include <stdio.h>
#include <stdlib.h>
#define INFINITE (1000000000)
typedef struct _GNode {
	int id;
	int weight;
	struct _GNode* next;
} GNode;
typedef struct {
	int size;
	GNode** tails;
} Graph;
Graph* GR_newGraph(const int size) {
	Graph* pgraph;
	if ((pgraph = malloc(sizeof(Graph))) == NULL) exit(1);
	if ((pgraph->tails = calloc(size, sizeof(GNode*))) == NULL) exit(1);
	pgraph->size = size;
	for (int i = 0; i < size; i++) {
		pgraph->tails[i] = malloc(sizeof(GNode));
		pgraph->tails[i]->id = 0;
		pgraph->tails[i]->next = pgraph->tails[i];
	}
	return pgraph;
}
void GR_deleteGraph(Graph* pgraph) {
	const int size = pgraph->size;
	for (int i = 0; i < size; i++) {
		GNode* _tail = pgraph->tails[i];
		GNode* cur = _tail;
		do {
			GNode* temp = cur;
			cur = cur->next;
			free(temp);
		} while (cur != _tail);
	}
	free(pgraph->tails);
	free(pgraph);
}
void GR_addEdge(const Graph* pgraph, const int from, const int to, const int weight) {
	GNode* newNode;
	newNode = malloc(sizeof(GNode));
	newNode->id = to;
	newNode->weight = weight;
	newNode->next = pgraph->tails[from]->next;
	pgraph->tails[from]->next = newNode;
	pgraph->tails[from] = newNode;
}
long long* solveBellmanFord(const Graph* pgraph, const int vtxSize, const int edgeSize, const int vtxStart) {
	long long* distances;
	if ((distances = calloc(vtxSize + 1, sizeof(long long))) == NULL) {
		exit(1);
	}
	for (int i = 1; i <= vtxSize; i++) {
		distances[i] = INFINITE;
	}
	distances[vtxStart] = 0;
	for (int i = 0; i < vtxSize - 1; i++) {
		for (int j = 1; j <= vtxSize; j++) {
			GNode* head = pgraph->tails[j]->next;
			GNode* cur = head->next;
			while (cur != head) {
				const int vtx = cur->id, weight = cur->weight;
				if (distances[j] != INFINITE && distances[vtx] > distances[j] + weight) {
					distances[vtx] = distances[j] + weight;
				}
				cur = cur->next;
			}
		}
	}
	for (int j = 1; j <= vtxSize; j++) {
		GNode* head = pgraph->tails[j]->next;
		GNode* cur = head->next;
		while (cur != head) {
			const int vtx = cur->id, weight = cur->weight;
			if (distances[j] != INFINITE && distances[vtx] > distances[j] + weight) {
				distances[0] = 1;
				goto ESC;
			}
			cur = cur->next;
		}
	} ESC:;
	return distances;
}
int main() {
	int N, M;
	long long* distances;
	Graph* graph;
	scanf("%d%d", &N, &M);
	graph = GR_newGraph(N + 1);
	for (int i = 0; i < M; i++) {
		int from, to, weight;
		scanf("%d%d%d", &from, &to, &weight);
		GR_addEdge(graph, from, to, weight);
	}
	distances = solveBellmanFord(graph, N, M, 1);
	if (distances[0]) {
		printf("-1\n");
	}
	else {
		for (int i = 2; i <= N; i++) {
			printf("%lld\n", distances[i] == INFINITE ? -1 : distances[i]);
		}
	}
	GR_deleteGraph(graph);
	free(distances);
}