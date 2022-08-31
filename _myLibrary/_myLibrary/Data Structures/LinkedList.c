#include <stdlib.h>
#include "LinkedList.h"
/*
typedef int Element;
typedef struct _LNode {
	struct _LNode* next;
	struct _LNode* prev;
	Element item;
} LNode;
typedef struct {
	LNode* tail;
	int len;
} LinkedList;
*/
LinkedList* LL_newList() {
	LinkedList* plist;
	if ((plist = malloc(sizeof(LinkedList))) == NULL) exit(1);
	plist->tail = NULL;
	plist->len = 0;
	return plist;
}
void LL_deleteList(LinkedList* plist) {
	LNode* const _head = plist->tail == NULL ? NULL : plist->tail->next;
	if (_head == NULL) {
		free(plist);
		return;
	}
	LNode* search = _head;
	LNode* temp;
	do {
		temp = search;
		search = search->next;
		free(temp);
	} while (search != _head);
	free(plist);
}
LNode* LL_node_getHead(const LinkedList* plist) {
	return plist->tail == NULL ? NULL : plist->tail->next;
}
LNode* LL_node_getTail(const LinkedList* plist) {
	return plist->tail;
}
int LL_isEmpty(const LinkedList* plist) {
	return plist->len == 0;
}
int LL_size(const LinkedList* plist) {
	return plist->len;
}
LNode* LL_insert(LinkedList* plist, int index, const Element item) {
	LNode* newNode;
	if ((newNode = malloc(sizeof(LNode))) == NULL) exit(1);
	const int _len = plist->len;
	newNode->item = item;
	if (_len == 0) {
		plist->tail = newNode;
		newNode->next = newNode;
		newNode->prev = newNode;
		plist->len++;
	}
	else {
		LNode* search = plist->tail;
		LNode* searchNext;
		if (index <= _len - index)
			for (int i = 0; i < index; i++) {
				search = search->next;
			}
		else if (index < _len) {
			index = _len - index;
			for (int i = 0; i < index; i++)
				search = search->prev;
		}
		else {
			plist->tail = newNode;
		}
		searchNext = search->next;
		newNode->next = searchNext;
		newNode->prev = search;
		searchNext->prev = newNode;
		search->next = newNode;
		plist->len++;
	}
	return newNode;
}
LNode* LL_node_insert(LinkedList* plist, LNode** pnode, const Element item) {
	LNode* newNode;
	if ((newNode = malloc(sizeof(LNode))) == NULL) exit(1);
	LNode* const _tail = plist->tail;
	newNode->item = item;
	if (_tail == NULL) {
		plist->tail = newNode;
		newNode->next = newNode;
		newNode->prev = newNode;
		plist->len++;
	}
	else {
		LNode* search = *pnode;
		if (search == _tail || search == NULL) {
			search = _tail;
			plist->tail = newNode;
		}
		LNode* searchNext = search->next;
		newNode->next = searchNext;
		newNode->prev = search;
		searchNext->prev = newNode;
		search->next = newNode;
		plist->len++;
	}
	return newNode;
}
Element LL_erase(LinkedList* plist, int index) {
	const int _len = plist->len;
	Element item;
	if (_len == 1) {
		item = plist->tail->item;
		free(plist->tail);
		plist->tail = NULL;
		plist->len--;
	}
	else {
		LNode* search = plist->tail;
		LNode* searchNext;
		if (index <= _len - index)
			for (int i = 0; i < index; i++) {
				search = search->next;
			}
		else if (index < _len - 1) {
			index = _len - index;
			for (int i = 0; i < index; i++)
				search = search->prev;
		}
		else {
			search = search->prev;
			plist->tail = search;
		}
		searchNext = search->next;
		searchNext->next->prev = search;
		search->next = searchNext->next;
		item = searchNext->item;
		free(searchNext);
		plist->len--;
	}
	return item;
}
Element LL_node_erase(LinkedList* plist, LNode** pnode) {
	const int _len = plist->len;
	Element item;
	if (_len == 1) {
		item = plist->tail->item;
		free(plist->tail);
		*pnode = NULL;
		plist->tail = NULL;
		plist->len--;
	}
	else {
		LNode* search = (*pnode)->prev;
		LNode* searchNext = *pnode;
		*pnode = (*pnode)->next;
		if (searchNext == plist->tail) plist->tail = searchNext->next;
		searchNext->next->prev = search;
		search->next = searchNext->next;
		item = searchNext->item;
		free(searchNext);
		plist->len--;
	}
	return item;
}
Element LL_read(const LinkedList* plist, int index) {
	LNode* search = plist->tail;
	const int _len = plist->len;
	if (index <= _len - index)
		for (int i = 0; i < index; i++) {
			search = search->next;
		}
	else if (index < _len - 1) {
		index = _len - index;
		for (int i = 0; i < index; i++)
			search = search->prev;
	}
	else {
		search = search->prev;
	}
	return search->next->item;
}
int LL_toArray(const LinkedList* plist, Element* dest, const int maxsize) {
	LNode* const _head = plist->tail == NULL ? NULL : plist->tail->next;
	if (_head == NULL) return 0;
	LNode* search = _head;
	int index = 0;
	do {
		dest[index++] = search->item;
		search = search->next;
	} while (search != _head && index < maxsize);
	return plist->len;
}



/*
* 2022.8.21 Sun
*/