#include <stdio.h>
#include <string.h>
#define MAXITEM (100001)
#define MAXBUF (10010)
#define MAXATTB (35)
#define MAXCOL (2000)
typedef int Element;
void _MergeSort_impl(Element* const orders, char const attributes[][MAXATTB], Element* const sorted, const int left, const int right) {
	if (left >= right) return;
	const int mid = left + (right - left) / 2;
	_MergeSort_impl(orders, attributes, sorted, left, mid);
	_MergeSort_impl(orders, attributes, sorted, mid + 1, right);
	int idx, first, second;
	idx = left, first = left, second = mid + 1;
	while (first <= mid && second <= right) {
		if ((strcmp(attributes[orders[first]], attributes[orders[second]])) < 1) sorted[idx++] = orders[first++];
		else sorted[idx++] = orders[second++];
	}
	while (first <= mid) {
		sorted[idx++] = orders[first++];
	}
	while (second <= right) {
		sorted[idx++] = orders[second++];
	}
	for (int i = left; i <= right; i++) {
		orders[i] = sorted[i];
	}
}
void MergeSort(Element* const orders, char const attributes[][MAXATTB], const int size) {
	static Element sorted[MAXITEM];
	for (int i = 0; i < size; i++) 
		orders[i] = i;
	_MergeSort_impl(orders, attributes, sorted, 0, size - 1);
}
void makeOutput(FILE* const _file1, FILE* const _file2) {
	int totalSize = 0;
	int attributeSize = 1;
	int pivot = -1;
	int buffer_len;
	static int items_orders[MAXITEM];
	static char items[MAXITEM][MAXCOL];
	static char attributes[MAXITEM][MAXATTB];
	static char buffer[MAXBUF];

	fscanf(_file1, "%d\n", &totalSize); //3
	if (totalSize <= 0) return;
	fgets(buffer, sizeof(buffer), _file1); //$
	fgets(buffer, sizeof(buffer), _file1); //P_Id:LastName*:FirstName:Address:City
	buffer_len = strlen(buffer);
	for (int i = 0; i < buffer_len; i++) {
		char picked = buffer[i];
		if (picked == ':') attributeSize++;
		else if (pivot == -1 && picked == '*') pivot = attributeSize - 1;
	}
	fgets(buffer, sizeof(buffer), _file1); //$
	for (int i = 0; i < totalSize; i++) {
		char* attbPtr;
		fgets(buffer, sizeof(buffer), _file1);
		buffer_len = strlen(buffer);
		if (buffer[buffer_len - 1] == '\n') {
			buffer[buffer_len - 1] = '\0';
		}
		strcpy(items[i], buffer);
		attbPtr = strtok(buffer, ":");
		for (int i = 1; i <= pivot; i++) {
			attbPtr = strtok(NULL, ":");
		}
		strcpy(attributes[i], attbPtr);
	}
	MergeSort(items_orders, attributes, totalSize);
	for (int i = 0; i < totalSize; i++) {
		fprintf(_file2, "%s\n", items[items_orders[i]]);
	}
}
int main() {
	const char* const name1 = "hw1_input.txt";
	const char* const name2 = "hw1_output.txt";
	FILE* _file1, * _file2;

	if ((_file1 = fopen(name1, "r")) == NULL) {
		perror(name1);
		return 1;
	}
	if ((_file2 = fopen(name2, "w")) == NULL) {
		perror(name2);
		return 1;
	}
	makeOutput(_file1, _file2);

	fclose(_file1);
	fclose(_file2);
}