#include "include/sorting.h"
#include "include/swap.h"
#include <stdio.h>
#include <string.h>

/*
 * Merge Sort.
 */

void merge_sort(int *array, size_t n){
	int *scratch;
	int i, j, curr_ind;

	if(n == 1) return;

	merge_sort(array, n/2);
	merge_sort(array + n/2, n/2 + n%2);
	
	/* Do the merge */
	scratch = malloc(n * sizeof(int));
	if(scratch == NULL){
		printf("Merge sort: Out of memory?\n");
		return;
	}

	i=0;
	j=n/2;
	curr_ind=0;
	while(i < n/2 || j < n){
		if(i >= n/2) /* Done with left subarray */
			scratch[curr_ind++] = array[j++];
		else if(j >= n) /* Done with right subarray */
			scratch[curr_ind++] = array[i++];
		else{ /* Not done with either subarray */
			if(array[i] < array[j]) scratch[curr_ind++] = array[i++];
			else scratch[curr_ind++] = array[j++];
		}
	}

	memcpy(array, scratch, (n * sizeof(int)));

	free(scratch);

        return;
}
