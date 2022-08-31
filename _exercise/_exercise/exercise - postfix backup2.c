#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef char Element;
typedef struct {
	Element* items;
	int capacity;
	int top;
} ArrayStack;
ArrayStack* AS_newStack(const int max) {
	ArrayStack* pstack;
	if ((pstack = malloc(sizeof(ArrayStack))) == NULL) exit(1);
	if ((pstack->items = calloc(max, sizeof(Element))) == NULL) exit(1);
	pstack->capacity = max;
	pstack->top = -1;
	return pstack;
}
void AS_deleteStack(ArrayStack* pstack) {
	if (pstack == NULL || pstack->items == NULL) exit(1);
	free(pstack->items);
	free(pstack);
}
int AS_isEmpty(const ArrayStack* pstack) {
	return pstack->top == -1;
}
int AS_size(const ArrayStack* pstack) {
	return pstack->top + 1;
}
Element AS_peek(const ArrayStack* pstack) {
	return pstack->items[pstack->top];
}
void AS_push(ArrayStack* pstack, const Element item) {
	pstack->items[++(pstack->top)] = item;
}
Element AS_pop(ArrayStack* pstack) {
	return pstack->items[(pstack->top)--];
}
char* getPostfix(const char* str) {
	const int _len = strlen(str);
	char* answer = calloc(_len + 1, sizeof(char));
	int ansIdx = 0;
	ArrayStack* stack = AS_newStack(_len);
	AS_push(stack, '(');
	for (int i = 0; i < _len; i++) {
		char ch = str[i];
		if ('A' <= ch && ch <= 'Z') {
			answer[ansIdx++] = ch;
		}
		//else if (ch == ')' || );
		else {
			int opPriorityFlag;
			char peek = AS_peek(stack);
			switch (ch) {
			case '(': opPriorityFlag = 1; break;
			case '*':
			case '/': //opPriorityFlag = 1; break;
				if (peek == '+' || peek == '-') opPriorityFlag = 1;
				else opPriorityFlag = 0;
				break;
			case ')':
			case '+':
			case '-': opPriorityFlag = 0; break;
			}
			if (opPriorityFlag) {
				AS_push(stack, ch);
			}
			else {
				while (AS_peek(stack) != '(') {
					answer[ansIdx++] = AS_pop(stack);
				}
				if (ch == ')') AS_pop(stack);
				else {
					AS_push(stack, ch);
				}
				printf("Now [%s]\n", answer);
			}
		}
	}
	int remain = AS_size(stack) - 1;
	for (int i = 0; i < remain; i++) {
		answer[ansIdx++] = AS_pop(stack);
	}
	AS_deleteStack(stack);
	return answer;
}
int main() {
	{
		freopen("i.txt", "r", stdin);
	}
	char str[102];
	char* answer;
	scanf("%s", str);
	answer = getPostfix(str);
	printf("%s\n", answer);
	free(answer);
}