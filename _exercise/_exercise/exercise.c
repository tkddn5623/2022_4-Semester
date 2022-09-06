#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
void countingSort(int arr[], int size) {
	int index = 0;
	int Positives[1001] = { 0 };
	int Negatives[1001] = { 0 };
	for (int i = 0; i < size; i++) {
		int read = arr[i];
		if (arr[i] >= 0) Positives[read]++;
		else Negatives[-read]++;
	}
	for (int i = 1000; i > 0; i--) {
		for (int j = 0; j < Negatives[i]; j++) {
			arr[index++] = -i;
		}
	}
	for (int i = 0; i <= 1000; i++) {
		for (int j = 0; j < Positives[i]; j++) {
			arr[index++] = i;
		}
	}
}
int main() {
	int len;
	int arr[1000] = { 0 };
	scanf("%d", &len);
	for (int i = 0; i < len; i++) {
		scanf("%d", &arr[i]);
	}
	countingSort(arr, len);
	for (int i = 0; i < len; i++) {
		printf("%d\n", arr[i]);
	}
}