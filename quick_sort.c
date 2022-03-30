#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "sorting.h"
#include "utils.h"

//
// note: this is untested on data sets that contain duplicates, and
// i'm quite sure it won't work on data sets that contain duplicates
//
int quick_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops) {
    uint8_t *_data, *ip, *jp, *pivp;
    size_t nl, nr;
    
    if (nelements <= 1)
        goto out;
    
    _data = (uint8_t *)data;
    pivp = _data + ((nelements / 2) * dsz);
    ip = _data;
    jp = _data + ((nelements - 1) * dsz);
    while (1) {
        while (ops->compare_fn(ip, pivp) < 0)
            ip += dsz;
        while (ops->compare_fn(jp, pivp) > 0)
            jp -= dsz;
        if (ip == jp)
            break;
        swap(ip, jp, dsz);
        if (ip == pivp)
            pivp = jp;
        else if (jp == pivp)
            pivp = ip;
    }
    
    assert((pivp == ip) && (pivp == jp));
    nl = (pivp - _data) / dsz;
    nr = nelements - nl - 1;
    quick_sort(_data, dsz, nl, ops);
    quick_sort(pivp + dsz, dsz, nr, ops);
    
out:
    return 0;
}
