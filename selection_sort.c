#include <stdint.h>

#include "sorting.h"
#include "utils.h"

int selection_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops) {
    uint8_t *curr, *min, *comp;
    int err;
    
    for (int i = 0; i < nelements; i++) {
        curr = ((uint8_t *)data) + (i * dsz);
        min = curr;
        for (int j = i + 1; j < nelements; j++) {
            comp = ((uint8_t *)data) + (j * dsz);
            if (ops->compare_fn(comp, min) < 0)
                min = comp;
        }
        if (min != curr)
            swap(min, curr, dsz);
    }
    
    return 0;
    
error_out:
    return err;
}
