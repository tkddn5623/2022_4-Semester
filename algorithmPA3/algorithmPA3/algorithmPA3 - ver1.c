#include <stdio.h>
#include <ctype.h>
typedef struct _BTree {
	int id;
	int leftc;
	int rightc;
	int priority;
	char ch;
} BTree;
typedef BTree Element;
typedef struct {
	Element item;
	int priority;
} HNode;
typedef struct {
	HNode nodes[260];
	int size;
} ArrayHeap;
int AH_isEmpty(const ArrayHeap* pheap) {
	return pheap->size == 0;
}
int AH_size(const ArrayHeap* pheap) {
	return pheap->size;
}
void AH_push(ArrayHeap* pheap, const Element item, const int priority) {
	HNode newNode;
	int index = pheap->size + 1;
	while (index > 1) {
		int parentIndex = index / 2;
		if (priority < pheap->nodes[parentIndex].priority ||
			(priority == pheap->nodes[parentIndex].priority && item.ch < pheap->nodes[parentIndex].item.ch)) {
			pheap->nodes[index] = pheap->nodes[parentIndex];
			index = parentIndex;
		}
		else break;
	}
	newNode.item = item;
	newNode.priority = priority;
	pheap->nodes[index] = newNode;
	pheap->size++;
}
Element AH_pop(ArrayHeap* pheap) {
	const int size = pheap->size;
	const Element topitem = pheap->nodes[1].item;
	const HNode last = pheap->nodes[size];
	int left, pickedChild, parentIndex = 1;
	while ((left = parentIndex * 2) <= size) {
		if (left == size) pickedChild = left;
		else if (pheap->nodes[left].priority < pheap->nodes[left + 1].priority ||
			(pheap->nodes[left].priority == pheap->nodes[left + 1].priority && pheap->nodes[left].item.ch < pheap->nodes[left + 1].item.ch))
			pickedChild = left;
		else pickedChild = left + 1;

		if (last.priority > pheap->nodes[pickedChild].priority ||
			(last.priority == pheap->nodes[pickedChild].priority && last.item.ch > pheap->nodes[pickedChild].item.ch)) {
			pheap->nodes[parentIndex] = pheap->nodes[pickedChild];
			parentIndex = pickedChild;
		}
		else break;
	}
	pheap->nodes[parentIndex] = last;
	pheap->size--;
	return topitem;
}
Element AH_peek_item(const ArrayHeap* pheap) {
	return pheap->nodes[1].item;
}
int AH_peek_priority(const ArrayHeap* pheap) {
	return pheap->nodes[1].priority;
}
char* mystrcpy(char* _Dest, const char* _Src) {
	int i;
	for (i = 0; _Src[i] != '\0'; i++) {
		_Dest[i] = _Src[i];
	}
	_Dest[i] = '\0';
	return _Dest;
}
char* mystrcat_ch(char* _Dest, const char ch) {
	int i = 0;
	while (_Dest[i] != '\0') {
		i++;
	}
	_Dest[i] = ch;
	_Dest[i + 1] = '\0';
	return _Dest;
}
int mytolower(const int _C) {
	if ('A' <= _C && _C <= 'Z') return _C + 0x20;
	else return _C;
}
int myisalpha(const int _C) {
	if (('A' <= _C && _C <= 'Z') || ('a' <= _C && _C <= 'z')) return 1;
	else return 0;
}
int myisupper(const int _C) {
	if ('A' <= _C && _C <= 'Z') return 1;
	else return 0;
}
void writefo1(FILE* fp, char codes[][260], BTree* ptree, int const root, const char* prevcode) {
	char code[260] = { 0 };
	if (ptree[root].leftc == -1) {
		fputc(ptree[root].ch, fp);
		mystrcpy(codes[ptree[root].ch], prevcode);
	}
	else {
		mystrcpy(code, prevcode);
		mystrcat_ch(code, '0');
		fputc('(', fp);
		writefo1(fp, codes, ptree, ptree[root].leftc, code);
		if (ptree[root].rightc != -1) {
			mystrcpy(code, prevcode);
			mystrcat_ch(code, '1');
			fputc(',', fp);
			writefo1(fp, codes, ptree, ptree[root].rightc, code);
		}
		fputc(')', fp);
	}
}
void writefo2(FILE* fp, char codes[][260], BTree* ptree, int const root, const char* prevcode) {

}
int readTree(FILE* fi, FILE* fo) {
	int tree_idx = 0;
	int tree_root = 0;
	fseek(fi, 0L, SEEK_SET);
	fseek(fo, 0L, SEEK_SET);
}
int makeTree(FILE* fi, FILE* fo) {
	ArrayHeap heap = { 0 };
	BTree tree[520] = { 0 };
	int tree_idx = 0;
	int tree_root = 0;
	int count[260] = { 0 };
	char codes[260][260] = { 0 };
	int priormax = -1;
	char ch;
	fseek(fi, 0L, SEEK_SET);
	fseek(fo, 0L, SEEK_SET);
	for (int i = 0; i < 520; i++) {
		tree[i].id = i;
		tree[i].leftc = -1;
		tree[i].rightc = -1;
		tree[i].ch = '\0';
		tree[i].priority = 100000;
	}
	while ((ch = fgetc(fi)) != EOF) {
		ch = mytolower(ch);
		count[ch]++;
	}
	for (int i = 'a'; i <= 'z'; i++) {
		if (count[i] > 0) {
			tree[tree_idx].ch = i;
			tree[tree_idx].priority = count[i];
			AH_push(&heap, tree[tree_idx], tree[tree_idx].priority);
			tree_idx++;
		}
	}
	tree_root = AH_peek_item(&heap).id;
	if (AH_size(&heap) == 1) {
		tree[tree_idx].ch = '\0';
		tree[tree_idx].leftc = AH_peek_item(&heap).id;
		tree_root = tree_idx;
		tree_idx++;
		writefo1(fo, codes, tree, tree_root, "");
	}
	else if (AH_size(&heap) > 1) {
		while (AH_size(&heap) > 1) {
			BTree t1, t2, temp;
			t1 = AH_pop(&heap);
			t2 = AH_pop(&heap);

			tree[tree_idx].ch = '\0';
			tree[tree_idx].leftc = t1.id;
			tree[tree_idx].rightc = t2.id;
			tree[tree_idx].id = tree_idx;
			tree[tree_idx].priority = t1.priority + t2.priority;
			AH_push(&heap, tree[tree_idx], tree[tree_idx].priority);
			if (priormax < tree[tree_idx].priority) {
				priormax = tree[tree_idx].priority;
				tree_root = tree_idx;
			}
			tree_idx++;
		}
		writefo1(fo, codes, tree, tree_root, "");
	}
	fseek(fi, 0L, SEEK_SET);
	fprintf(fo, "HEADEREND");
	while ((ch = fgetc(fi)) != EOF) {
		ch = mytolower(ch);
		fprintf(fo, "%s", codes[ch]);
	}
	return 0;
}
int main() {
	const char* const path1 = "hw3_input.txt";
	const char* const path2 = "hw3_output1.txt";
	const char* const path3 = "hw3_output2.txt";
	FILE* const fi = fopen(path1, "r");
	FILE* const fo1 = fopen(path2, "w+");
	FILE* const fo2 = fopen(path3, "w");
	if (!fi | !fo1 | !fo2) {
		printf("File open error\n");
		return 1;
	}
	makeTree(fi, fo1);
	readTree(fo1, fo2);
	fclose(fi);
	fclose(fo1);
	fclose(fo2);
}