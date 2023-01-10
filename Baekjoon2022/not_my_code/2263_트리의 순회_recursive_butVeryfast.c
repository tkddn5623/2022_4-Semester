https://www.acmicpc.net/source/5145144
#include <stdio.h>
int inorder[100001];
int postorder[100001];
int position[100001];

void solve(int in_s, int in_e, int po_s, int po_e) {
    if (in_s > in_e || po_s > po_e) return;
    int root = postorder[po_e];
    printf("%d ", root);
    int p = position[root];
    int left = p - in_s;
    solve(in_s, p-1, po_s, po_s+left-1);
    solve(p+1, in_e, po_s+left, po_e-1);
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i=0; i<n; i++) scanf("%d", &inorder[i]);
    for (int i=0; i<n; i++) scanf("%d", &postorder[i]);
    for (int i=0; i<n; i++) {
        position[inorder[i]] = i;
    }
    solve(0,n-1,0,n-1);
    printf("\n");
    return 0;
}