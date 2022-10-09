#include <stdio.h>
#include <string.h>
#define MAXITEM (100001)
#define MAXBUF (10010)
#define MAXATB (32)
#define MAXCOL (1600)
typedef int Element;
void _MergeSort_impl(Element* const list, Element* const sorted, const int left, const int right) {
	if (left >= right) return;
	const int mid = left + (right - left) / 2;
	_MergeSort_impl(list, sorted, left, mid);
	_MergeSort_impl(list, sorted, mid + 1, right);
	int idx, first, second;
	idx = left, first = left, second = mid + 1;
	while (first <= mid && second <= right) {
		if (list[first] <= list[second]) sorted[idx++] = list[first++];
		else sorted[idx++] = list[second++];
	}
	while (first <= mid) {
		sorted[idx++] = list[first++];
	}
	while (second <= right) {
		sorted[idx++] = list[second++];
	}
	for (int i = left; i <= right; i++) {
		list[i] = sorted[i];
	}
}
Element* MergeSort(Element* list, const int size) {
	static Element sorted[MAXITEM];
	_MergeSort_impl(list, sorted, 0, size - 1);
	return list;
}

void _mySort(FILE* const _file1, FILE* const _file2, char items[][MAXCOL], const int size) {

}
void makeOutput(FILE* const _file1, FILE* const _file2) {
	int totalSize = 0;
	int attributeSize = 1;
	int pivot = -1;
	int buffer_len;
	static int items_order[MAXITEM];
	static char items[MAXITEM][MAXCOL];
	static char attributes[MAXITEM][MAXATB];
	char buffer[MAXBUF];

	fscanf(_file1, "%d\n", &totalSize); //3
	if (totalSize <= 0) return 1;
	fgets(buffer, sizeof(buffer), _file1); //$
	fprintf(_file2, "%d\n%s", totalSize, buffer);

	fgets(buffer, sizeof(buffer), _file1); //P_Id:LastName*:FirstName:Address:City
	fprintf(_file2, "%s", buffer);
	buffer_len = strlen(buffer);
	for (int i = 0; i < buffer_len; i++) {
		char picked = buffer[i];
		if (picked == ':') attributeSize++;
		else if (pivot == -1 && picked == '*') pivot = attributeSize - 1;
	}
	fgets(buffer, sizeof(buffer), _file1); //$
	fprintf(_file2, "%s", buffer);
	for (int i = 0; i < totalSize; i++) {
		char* bufPtr = buffer;
		fgets(buffer, sizeof(buffer), _file1);
		printf("[buffs [%s]\n]", buffer);
		strcpy(items[i], buffer);
		puts("---");
		printf("itesm %d is [%s]\n", i, items[i]);
		buffer_len = strlen(buffer);
		if (buffer[buffer_len - 1] == '\n') {
			buffer[buffer_len - 1] = '0';
			buffer_len--;
		}
		

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