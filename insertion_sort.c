#include "include/sorting.h"
#include "include/swap.h"

/*
 * Insertion Sort.
 */

void insertion_sort(int *array, size_t n){
	int i, j;

	for(i=1; i<n; i++){
		j = i;
		while(j > 0){
			if(array[j] < array[j - 1]){
				swap(&array[j], &array[j - 1]);
				j--;
			}
			else break;
		}
	}

	return;
}
