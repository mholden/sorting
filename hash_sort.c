#include "include/sorting.h"
#include <stdio.h>
#include <string.h>

/*
 * Hash Sort.
 */

void hash_sort(int *array, size_t n){
	/*
	 * Uses the fact that we only generate numbers
	 * in the range 0 - (2n - 1) for our array.
	 */
	int i, j;
	int *table;

	table = malloc(2*n * sizeof(int));
	if(table == NULL){
		printf("Hash sort: Out of memory?\n");
		return;
	}

	memset(table, 0, (2*n * sizeof(int)));

	for(i=0; i<n; i++){
		table[array[i]]++;
	}

	j=0;
	for(i=0; i<2*n; i++){
		while(table[i] > 0){
			array[j++] = i;
			table[i]--;
		}
	}

	free(table);

        return;
}
