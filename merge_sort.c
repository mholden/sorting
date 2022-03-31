#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

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

// multi-threaded version:

typedef struct merge_sort_mt_ctx {
    uint8_t *data;
    size_t dsz;
    size_t nelements;
    sorting_ops_t *ops;
    uint8_t *buf;
    long ncpu;
    long nthreads;
} merge_sort_mt_ctx_t;

static void *_merge_sort_mt(void *arg) {
    uint8_t *data, *buf;
    size_t dsz, nelements, nl, nr;
    sorting_ops_t *ops;
    long ncpu, nthreads;
    bool do_threads = false;
    pthread_t t1, t2;
    merge_sort_mt_ctx_t *ctx, ctxl, ctxr;
    int err;
    void *res1, *res2;
    
    ctx = (merge_sort_mt_ctx_t *)arg;
    data = ctx->data;
    dsz = ctx->dsz;
    nelements = ctx->nelements;
    ops = ctx->ops;
    buf = ctx->buf;
    ncpu = ctx->ncpu;
    nthreads = ctx->nthreads;
    
    if (nelements <= 1)
        goto out;
    
    nr = nelements / 2;
    nl = nelements - nr;
    
    if (nthreads < ncpu) {
        ctx->nthreads += 2; // we'll spawn 2 more threads if our cpus are under-utilized
        do_threads = true;
    }
    
    memcpy(&ctxl, ctx, sizeof(merge_sort_mt_ctx_t));
    ctxl.nelements = nl;
    
    memcpy(&ctxr, ctx, sizeof(merge_sort_mt_ctx_t));
    ctxr.data = data + (nl * dsz);
    ctxr.nelements = nr;
    ctxr.buf = buf + (nl * dsz);
    
    if (do_threads) {
        err = pthread_create(&t1, NULL, _merge_sort_mt, &ctxl);
        if (err)
            goto error_out;
        
        err = pthread_create(&t2, NULL, _merge_sort_mt, &ctxr);
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
        assert(_merge_sort_mt(&ctxl) == NULL);
        assert(_merge_sort_mt(&ctxr) == NULL);
    }
    
    merge(data, buf, nl, nr, dsz, ops);
    
out:
    return NULL;
    
error_out:
    return (void *)(intptr_t)err;
}

int merge_sort_mt(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops) {
    uint8_t *_data, *buf = NULL;
    size_t nl, nr;
    long ncpu;
    merge_sort_mt_ctx_t ctxl, ctxr;
    pthread_t t1, t2;
    int err;
    void *res1, *res2;
    
    ncpu = sysconf(_SC_NPROCESSORS_ONLN);
    if (ncpu < 0) {
        err = errno;
        goto error_out;
    }
    
    _data = (uint8_t *)data;
    buf = malloc(nelements * dsz);
    if (!buf) {
        err = ENOMEM;
        goto error_out;
    }
    
    nr = nelements / 2;
    nl = nelements - nr;
    
    memset(&ctxl, 0, sizeof(merge_sort_mt_ctx_t));
    ctxl.data = _data;
    ctxl.dsz = dsz;
    ctxl.nelements = nl;
    ctxl.ops = ops;
    ctxl.buf = buf;
    ctxl.ncpu = ncpu;
    ctxl.nthreads = 1;
    if (ncpu > 1)
        ctxl.nthreads += 2; // will spawn 2 threads below
    
    memcpy(&ctxr, &ctxl, sizeof(merge_sort_mt_ctx_t));
    ctxr.data = _data + (nl * dsz);
    ctxr.nelements = nr;
    ctxr.buf = buf + (nl * dsz);
    
    if (ncpu > 1) {
        err = pthread_create(&t1, NULL, _merge_sort_mt, &ctxl);
        if (err)
            goto error_out;
        
        err = pthread_create(&t2, NULL, _merge_sort_mt, &ctxr);
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
        assert(_merge_sort_mt(&ctxl) == NULL);
        assert(_merge_sort_mt(&ctxr) == NULL);
    }
    
    merge(_data, buf, nl, nr, dsz, ops);
    
    free(buf);
    
    return 0;
    
error_out:
    if (buf)
        free(buf);
    
    return err;
}

