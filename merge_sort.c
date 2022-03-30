#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "sorting.h"

// merge buf array(s) into data array
static void merge(uint8_t *data, uint8_t *buf, size_t nl, size_t nr, size_t dsz, sorting_ops_t *ops) {
    uint8_t *l, *r;
    size_t i, j, k;
    
    memcpy(buf, data, (nl + nr) * dsz);
    
    i = 0;
    j = nl;
    k = 0;
    while ((i < nl) || (j < (nl + nr))) {
        assert(k < (nl + nr));
        l = buf + (i * dsz);
        r = buf + (j * dsz);
        if ((i < nl) && (j < (nl + nr))) { // data left to merge in both sub-buffers
            if (ops->compare_fn(l, r) < 0) {
                memcpy(data + (k * dsz), l, dsz);
                i++;
            } else {
                memcpy(data + (k * dsz), r, dsz);
                j++;
            }
        } else if (i < nl) { // data left in left buffer but not in right
            memcpy(data + (k * dsz), l, dsz);
            i++;
        } else { // data left in right buffer but not in left
            assert(j < (nl + nr));
            memcpy(data + (k * dsz), r, dsz);
            j++;
        }
        k++;
    }
    assert(k == (nl + nr));
    
    return;
}

static void _merge_sort(uint8_t *data, size_t dsz, size_t nelements, sorting_ops_t *ops, uint8_t *buf) {
    size_t nl, nr;
    
    if (nelements <= 1)
        goto out;
    
    nr = nelements / 2;
    nl = nelements - nr;
    _merge_sort(data, dsz, nl, ops, buf);
    _merge_sort(data + (nl * dsz), dsz, nr, ops, buf + (nl * dsz));
    merge(data, buf, nl, nr, dsz, ops);
    
out:
    return;
}

int merge_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops) {
    uint8_t *_data, *buf = NULL;
    size_t nl, nr;
    int err;
    
    _data = (uint8_t *)data;
    buf = malloc(nelements * dsz);
    if (!buf) {
        err = ENOMEM;
        goto error_out;
    }
    
    nr = nelements / 2;
    nl = nelements - nr;
    _merge_sort(_data, dsz, nl, ops, buf);
    _merge_sort(_data + (nl * dsz), dsz, nr, ops, buf + (nl * dsz));
    merge(_data, buf, nl, nr, dsz, ops);
    
    free(buf);
    
    return 0;
    
error_out:
    if (buf)
        free(buf);
    
    return err;
}
