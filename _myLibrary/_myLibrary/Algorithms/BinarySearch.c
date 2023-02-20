#include <stdlib.h>
int binary_lower(const int arr[], int size, const int key) {
	int pos = -1, left = 0, mid;
	size = size - 1;
	while (left <= size) {
		mid = left + (size - left) / 2;
		if (arr[mid] == key) {
			pos = mid;
			size = mid - 1;
		}
		else if (arr[mid] < key) left = mid + 1;
		else size = mid - 1;
	}
	return pos;
}


/*
* 2023.2.20 Mon.
* binary_upper is skipped. just change one line then it works.
* binary_lower_greater can be implemented by moving pos assignment and changing of assignment in line 9
* binary_lower_greater_equal can be implemented by same ways.
*/