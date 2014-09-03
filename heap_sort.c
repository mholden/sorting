#include "include/sorting.h"
#include "include/swap.h"
#include <stdio.h>

/*
 * Heap Sort.
 */

void upheap(int *heap, int index){
	int child_ind, parent_ind;

	if(index == 0) return;

	child_ind = index;
	parent_ind = (child_ind - 1) / 2;

	if(heap[child_ind] < heap[parent_ind]){
		swap(&heap[child_ind], &heap[parent_ind]);
		upheap(heap, parent_ind);
	}
	else return;
}

void downheap(int *heap, int index, size_t size){
	int parent_ind, lch_ind, rch_ind;

	if(size == 0) return;

	parent_ind = index;
	lch_ind = (2 * parent_ind) + 1;
	rch_ind = (2 * parent_ind) + 2;

	if(lch_ind > (size - 1)) return; /* We have no children */
	else if(rch_ind > (size - 1)){ /* We only have a left child */
		if(heap[parent_ind] > heap[lch_ind]){
			swap(&heap[parent_ind], &heap[lch_ind]);
		}
		return;
	}

	/* We have two children */
	if(heap[parent_ind] > heap[lch_ind]){
		if(heap[parent_ind] > heap[rch_ind]){ 
			/* We're larger than both children */
			if(heap[lch_ind] < heap[rch_ind]){
				swap(&heap[lch_ind], &heap[parent_ind]);
				downheap(heap, lch_ind, size);
			}
			else{
				swap(&heap[rch_ind], &heap[parent_ind]);
                                downheap(heap, rch_ind, size);
			}
		}
		else{ /* We're larger than only our left child */
			swap(&heap[lch_ind], &heap[parent_ind]);
			downheap(heap, lch_ind, size);
		}
	}
	else if(heap[parent_ind] > heap[rch_ind]){
		/* We're larger than only our right child */
		swap(&heap[rch_ind], &heap[parent_ind]);
		downheap(heap, rch_ind, size);
	}

	/* We're smaller than both our children */

	return;
}

void heap_sort(int *array, size_t n){
	/*
	 * Okay, heap is represented as an array.
	 * 	index n has left child 2n+1, right child 2n+2
	 *	index n has parent (n-1)/2
	 *
	 * Really just need to make a heap, and then empty the
	 * heap. To make heap, need upheap. To empty, need
	 * downheap.
	 *
	 * Upheap:
	 * 	insert at end of 'complete tree'/array
	 * 	look at parent, if < parent, swap
	 * 	this can/should be recursive
	 *	stop at root or when parent < you
	 *
	 * Downheap: 
	 * 	remove root node, and replace it with end of
	 *	'complete tree'/array.
	 * 	look at children:
	 *	  if > both, swap with whichever is smallest
	 *	  else swap with one that is < you
	 *	  stop when no children or both > you
	 */
	int *scratch;
	int curr_size, i;
	
	scratch = malloc(n * sizeof(int));
	if(scratch == NULL){
		printf("Heap sort: Out of memory?\n");
		return;
	}

	curr_size = 0;

	/* Build heap */
	i = 0;
	while(i < n){
		scratch[curr_size++] = array[i];
		upheap(scratch, curr_size - 1);
		i++;
	}

	/* Empty heap */
	i = 0;
	while(i < n){
		array[i] = scratch[0];
		swap(&scratch[0], &scratch[curr_size - 1]);
		curr_size--;
		downheap(scratch, 0, curr_size);
		i++;
	}

	free(scratch);

        return;
}
