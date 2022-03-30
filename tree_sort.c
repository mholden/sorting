#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "sorting.h"
#include "avl_tree.h"

typedef struct tree_sort_cb_ctx {
    void *data;
    size_t dsz;
    size_t current;
} tree_sort_cb_ctx_t;

static int tree_sort_cb(void *data, void *ctx, bool *stop) {
    tree_sort_cb_ctx_t *tsc = (tree_sort_cb_ctx_t *)ctx;
    
    memcpy(((uint8_t *)tsc->data) + (tsc->current * tsc->dsz), data, tsc->dsz);
    tsc->current++;
    
    return 0;
}

int tree_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops) {
    void *tdata = NULL;
    avl_tree_t *at = NULL;
    avl_tree_ops_t aops;
    tree_sort_cb_ctx_t ctx;
    int err;
    
    tdata = malloc(nelements * dsz);
    if (!tdata) {
        err = ENOMEM;
        goto error_out;
    }
    
    memcpy(tdata, data, nelements * dsz);
    
    memset(&aops, 0, sizeof(avl_tree_ops_t));
    aops.ato_compare_fn = ops->compare_fn;
    
    at = at_create(&aops);
    if (!at) {
        err = ENOMEM;
        goto error_out;
    }
    
    // insert all elements into an avl-tree
    for (int i = 0; i < nelements; i++) {
        err = at_insert(at, (void *)(((uint8_t *)tdata) + (i * dsz)));
        if (err)
            goto error_out;
    }
    
    memset(&ctx, 0, sizeof(tree_sort_cb_ctx_t));
    ctx.data = data;
    ctx.dsz = dsz;
    
    // then iterate the tree in order, dumping each element back into 'data'
    err = at_iterate(at, tree_sort_cb, (void *)&ctx);
    if (err)
        goto error_out;
    
    at_destroy(at);
    free(tdata);
    
    return 0;
    
error_out:
    if (tdata)
        free(tdata);
    if (at)
        at_destroy(at);
    
    return err;
}
