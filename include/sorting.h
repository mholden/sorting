#ifndef _SORTING_H_
#define _SORTING_H_

#include <stddef.h>

typedef struct sorting_ops {
    int (*compare_fn)(void *data1, void *data2);
    void (*dump_data_fn)(void *data, size_t nelements);
} sorting_ops_t;

int selection_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops);
int insertion_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops);
int merge_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops);
int heap_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops);
int tree_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops);
int quick_sort(void *data, size_t dsz, size_t nelements, sorting_ops_t *ops);

#endif // _SORTING_H_
