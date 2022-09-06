#ifndef ELEMENT_TYPE
#define ELEMENT_TYPE
typedef int Element;
#endif

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
typedef struct _LNode {
	struct _LNode* next;
	struct _LNode* prev;
	Element item;
} LNode;
typedef struct {
	LNode* tail;
	int len;
} LinkedList;
LinkedList* LL_newList();
void LL_deleteList(LinkedList* plist);
LNode* LL_node_getHead(const LinkedList* plist);
LNode* LL_node_getTail(const LinkedList* plist);
int LL_isEmpty(const LinkedList* plist);
int LL_size(const LinkedList* plist);
LNode* LL_insert(LinkedList* plist, int index, const Element item);
LNode* LL_node_insert(LinkedList* plist, LNode** pnode, const Element item);
Element LL_erase(LinkedList* plist, int index);
Element LL_node_erase(LinkedList* plist, LNode** pnode);
Element LL_read(const LinkedList* plist, int index);
int LL_toArray(const LinkedList* plist, Element* dest, const int maxsize);
#endif