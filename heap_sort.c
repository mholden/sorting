#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "sorting.h"
#include "utils.h"

//
// copied and modified from heap implementation in
// data_structs project so that we can do this in place:
//
static void heap_downheap(uint8_t *heap, size_t ind, size_t nelements, size_t dsz, sorting_ops_t *ops) {
    size_t cind1, cind2, scind = 0;
    uint8_t *parent, *child1 = NULL, *child2 = NULL, *swap_child;
    
    if (nelements == 0)
        return;
    
    cind1 = (ind * 2) + 1;
    cind2 = cind1 + 1;
    
    parent = heap + (ind * dsz);
    if (cind1 < nelements)
        child1 = heap + (cind1 * dsz);
    if (cind2 < nelements)
        child2 = heap + (cind2 * dsz);
    
    if (child1 == NULL && child2 == NULL)
        return;
    
    if (child1 == NULL)
        scind = cind2;
    else if (child2 == NULL)
        scind = cind1;
    
    if (!scind) { // neither child1 nor child2 are NULL
        if (ops->compare_fn(child1, child2) > 0)
            scind = cind1;
        else
            scind = cind2;
    }
    
    swap_child = heap + (scind * dsz);
    
    if (ops->compare_fn(swap_child, parent) > 0) { // swap and continue
        swap(swap_child, parent, dsz);
        heap_downheap(heap, scind, nelements, dsz, ops);
    }
    
    return;
}

static void heap_upheap(uint8_t *heap, size_t ind, size_t dsz, sorting_ops_t *ops) {
    size_t pind;
    uint8_t *child, *parent;
    
    if (ind == 0)
        goto out;
    
    pind = (ind - 1) / 2;
    child = heap + (ind * dsz);
    parent = heap + (pind * dsz);
    
    if (ops->compare_fn(parent, child) < 0) { // swap and continue
        swap(parent, child, dsz);
        if (pind)
            heap_upheap(heap, pind, dsz, ops);
    }
    
out:
    return;
}

int heap_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops) {
    uint8_t *heap;
    
    heap = (uint8_t *)data;
    for (size_t i = 1; i < nelements; i++) {
        //
        // 'insert' all elements into the heap. we 'fake' the
        // heap size here, maintaining a heap between indices
        // 0 and i until the entire data set is a heap
        //
        heap_upheap(heap, i, dsz, ops);
    }
    for (size_t i = 0; i < nelements; i++) {
        //
        // the heap is a max heap. we want the data sorted
        // min to max. so pop the heap top and put it at the
        // end of our data (by swapping it with the last element),
        // and then downheap on that last element (which is
        // now the heap top) to make sure the heap maintains
        // its properties
        //
        swap(heap, heap + ((nelements - i - 1) * dsz), dsz);
        heap_downheap(heap, 0, (nelements - i - 1), dsz, ops);
    }
    
    return 0;
}
