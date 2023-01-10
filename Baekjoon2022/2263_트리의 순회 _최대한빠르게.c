#include <stdio.h>
#include <stdlib.h>
int Inorder[100000] = { 0 };
int Inorder_map[100000] = { 0 };
int Postorder[100000] = { 0 };

void _impl_getPreorder(const int io_bias, const int po_bias, const int N) {
	const int root = Postorder[po_bias + N - 1];
	const int io_mid = Inorder_map[root];
	printf("%d ", root);
	if (io_mid - io_bias > 0) _impl_getPreorder(io_bias, po_bias, io_mid - io_bias);
	if (N - io_mid + io_bias - 1 > 0) _impl_getPreorder(io_bias + io_mid - io_bias + 1, po_bias + io_mid - io_bias, N - io_mid + io_bias - 1);
}
int main() {
	int N;
	scanf("%d", &N);
	for (int i = 0; i < N; i++) { scanf("%d", &Inorder[i]); Inorder_map[Inorder[i]] = i; }
	for (int i = 0; i < N; i++) { scanf("%d", &Postorder[i]); }
	_impl_getPreorder(0, 0, N);
}