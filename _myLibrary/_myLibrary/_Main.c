#include <stdio.h>
#include <stdlib.h>
#include "Algorithms/BinarySearch.h"
int main() {
	int arr[] = { -5, -4, -3, -3, -1, 0, 0, 0, 1, 2, 2, 4, 7, 8, 8, 8, 10 };
	printf("[%d]\n", binary_lower(arr, sizeof(arr), 0));
	
}

