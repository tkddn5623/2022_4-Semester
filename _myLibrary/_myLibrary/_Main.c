#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Data Structures/LinkedList.h"
int main() {
	
	LinkedList* list = LL_newList();
	LL_insert(list, 0, 1111);
	LL_insert(list, 0, 2222);
	LL_insert(list, 0, 3333);
	LL_insert(list, 555, 4444);
	printf("%d\n", LL_node_getHead(list));
	LL_insert(list, 0, 3333);
	LL_deleteList(list);
}

