#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

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

// multi-threaded version:

typedef struct quick_sort_mt_ctx {
    uint8_t *data;
    size_t dsz;
    size_t nelements;
    sorting_ops_t *ops;
    long ncpu;
    long nthreads;
} quick_sort_mt_ctx_t;

static void *_quick_sort_mt(void *arg) {
    uint8_t *data, *ip, *jp, *pivp;
    size_t dsz, nelements, nl, nr;
    sorting_ops_t *ops;
    long ncpu, nthreads;
    pthread_t t1, t2;
    quick_sort_mt_ctx_t *ctx, ctxl, ctxr;
    bool do_threads = false;
    int err;
    void *res1, *res2;
    
    ctx = (quick_sort_mt_ctx_t *)arg;
    data = ctx->data;
    dsz = ctx->dsz;
    nelements = ctx->nelements;
    ops = ctx->ops;
    ncpu = ctx->ncpu;
    nthreads = ctx->nthreads;
    
    if (nelements <= 1)
        goto out;
    
    pivp = data + ((nelements / 2) * dsz);
    ip = data;
    jp = data + ((nelements - 1) * dsz);
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
    nl = (pivp - data) / dsz;
    nr = nelements - nl - 1;
    
    if (nthreads < ncpu) {
        ctx->nthreads += 2; // we'll spawn 2 more threads if our cpus are under-utilized
        do_threads = true;
    }
    
    memcpy(&ctxl, ctx, sizeof(quick_sort_mt_ctx_t));
    ctxl.nelements = nl;
    
    memcpy(&ctxr, ctx, sizeof(quick_sort_mt_ctx_t));
    ctxr.nelements = nr;
    ctxr.data = pivp + dsz;
    
    if (do_threads) {
        err = pthread_create(&t1, NULL, _quick_sort_mt, &ctxl);
        if (err)
            goto error_out;
        
        err = pthread_create(&t2, NULL, _quick_sort_mt, &ctxr);
        if (err)
            goto error_out;
        
        err = pthread_join(t1, &res1);
        if (err)
            goto error_out;
        if (res1) {
            err = (int)(intptr_t)res1;
            goto error_out;
        }
        
        err = pthread_join(t2, &res2);
        if (err)
            goto error_out;
        if (res2) {
            err = (int)(intptr_t)res2;
            goto error_out;
        }
    } else {
        // shouldn't ever return error in non-threaded case
        assert(_quick_sort_mt(&ctxl) == NULL);
        assert(_quick_sort_mt(&ctxr) == NULL);
    }
    
out:
    return NULL;
    
error_out:
    return (void *)(intptr_t)err;
}

int quick_sort_mt(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops) {
    long ncpu;
    quick_sort_mt_ctx_t ctx;
    int err;
    
    ncpu = sysconf(_SC_NPROCESSORS_ONLN);
    if (ncpu < 0) {
        err = errno;
        goto error_out;
    }
    
    memset(&ctx, 0, sizeof(quick_sort_mt_ctx_t));
    ctx.data = (uint8_t *)data;
    ctx.dsz = dsz;
    ctx.nelements = nelements;
    ctx.ops = ops;
    ctx.ncpu = ncpu;
    ctx.nthreads = 1;
    
    err = (int)(intptr_t)_quick_sort_mt(&ctx);
    if (err)
        goto error_out;
    
out:
    return 0;
    
error_out:
    return err;
}
