#include "include/sorting.h"
#include "include/swap.h"
#include <stdlib.h>

/*
 * Quick Sort.
 */

void quick_sort(int *array, size_t n){
	int piv, i, j, ind;

	if(n <= 1) return;

	piv = rand() % n;

	i = 0;
	j = n - 1;
	while(1){
		while(array[i] <= array[piv] && i < n) i++;
		while(array[j] >= array[piv] && j > 0) j--;
		if(j <= i){
			/*
			 * if j>pivot, swap j and pivot
			 * if i<pivot, swap i and pivot
			 * else don't swap any
			 */
			if(j > piv){
				swap(&array[j], &array[piv]);
				ind = j;
			}
			else if(i < piv){
				swap(&array[i], &array[piv]);
				ind = i;
			}
			else ind = piv;
			break;
		}
		else swap(&array[i], &array[j]);
	}

	quick_sort(array, ind);
	quick_sort(array + (ind + 1), n - (ind + 1));

        return;
}
