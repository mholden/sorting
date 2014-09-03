#include "include/sorting.h"
#include "include/swap.h"

/*
 * Selection Sort, recursive implementation.
 */

void selection_sort(int *array, size_t n){
	int min_index, i;

	if(n == 1) return;

	min_index = 0;
	i = 1;
	while(i < n){
		if(array[i] < array[min_index]) min_index = i;
		i++;
	}

	if(min_index != 0) swap(&array[min_index], &array[0]);

	selection_sort(array + 1, n - 1);

	return;
}
