#include "include/sorting.h"
#include "include/swap.h"

/*
 * Bubble Sort, recursive implementation.
 */

void bubble_sort(int *array, size_t n){
        int i, swapped_flag;

        if(n == 1) return;

        i = 1;
	swapped_flag = 0;
        while(i < n){
                if(array[i-1] > array[i]){
			swap(&array[i], &array[i-1]);
                	swapped_flag = 1;
		}
		i++;
        }

	if(!swapped_flag) return; /* Array already sorted */

        bubble_sort(array, n-1);
        return;
}
