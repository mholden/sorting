#include <stdint.h>

#include "sorting.h"
#include "utils.h"

int insertion_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops) {
    uint8_t *curr, *comp;
    
    for (int i = 1; i < nelements; i++) {
        curr = (uint8_t *)data + (i * dsz);
        for (int j = i - 1; j >= 0; j--) {
            comp = (uint8_t *)data + (j * dsz);
            if (ops->compare_fn(curr, comp) >= 0)
                break;
            swap(curr, comp, dsz);
            curr = comp;
        }
    }
    
    return 0;
}
