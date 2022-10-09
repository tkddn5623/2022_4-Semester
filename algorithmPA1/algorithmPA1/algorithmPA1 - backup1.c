#include <stdio.h>
#include <string.h>
#define MAXITEM (1000001)
#define MAXCOL (31)
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
	char items[55][MAXCOL];
	char buffer[10000];

	fscanf(_file1, "%d\n", &totalSize);
	if (totalSize <= 0) return 1;
	fgets(buffer, sizeof(buffer), _file1);
	fprintf(_file2, "%d\n%s", totalSize, buffer);

	fgets(buffer, sizeof(buffer), _file1);
	fprintf(_file2, "%s", buffer);
	buffer_len = strlen(buffer);;
	printf("[%s]\n", buffer);
	printf("P : %d, ATS : %d\n", pivot, attributeSize);
	for (int i = 0; i < buffer_len; i++) {
		char picked = buffer[i];
		if (picked == ':') attributeSize++;
		else if (pivot == -1 && picked == '*') pivot = attributeSize - 1;
	}
	printf("P : %d, ATS : %d\n", pivot, attributeSize);
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