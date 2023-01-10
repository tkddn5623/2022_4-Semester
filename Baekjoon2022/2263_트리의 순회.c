#include <stdio.h>
#include <stdlib.h>
void _impl(int preorder[], const int inorder[], const int postorder[], const int N) {
	static int idx = 0;
	const int root = postorder[N - 1];
	preorder[idx++] = root;
	for (int i = 0; i < N; i++) {
		if (inorder[i] == root) {
			if (i > 0) _impl(preorder, inorder, postorder, i);
			if (N - i - 1 > 0) _impl(preorder, inorder + i + 1, postorder + i, N - i - 1);
			return;
		}
	}
}
int* getPreorder(const int inorder[], const int postorder[], const int N) {
	int* preorder;
	if ((preorder = calloc(N, sizeof(int))) == NULL) {
		exit(1);
	}
	_impl(preorder, inorder, postorder, N);
	return preorder;
}
int main() {
	int N;
	int inorder[100000] = { 0 };
	int postorder[100000] = { 0 };
	int* preorder;
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &inorder[i]);
	}
	for (int i = 0; i < N; i++) {
		scanf("%d", &postorder[i]);
	}
	preorder = getPreorder(inorder, postorder, N);
	for (int i = 0; i < N; i++) {
		printf("%d ", preorder[i]);
	}
	free(preorder);
}