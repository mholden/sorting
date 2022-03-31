#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include "sorting.h"
#include "hash_table.h"

static double diff_timespec(struct timespec tend, struct timespec tstart) {
    return (tend.tv_sec - tstart.tv_sec) + ((tend.tv_nsec - tstart.tv_nsec) / 1e9);
}

typedef struct test_sorts_data {
    uint64_t tsd_key;
} test_sorts_data_t;

static int _test_sorts_compare(void *data1, void *data2) {
    test_sorts_data_t *tsd1, *tsd2;
    
    tsd1 = (test_sorts_data_t *)data1;
    tsd2 = (test_sorts_data_t *)data2;
    
    if (tsd1->tsd_key < tsd2->tsd_key)
        return -1;
    else if (tsd1->tsd_key > tsd2->tsd_key)
        return 1;
    else // ==
        return 0;
}

static int test_sorts_compare(const void *data1, const void *data2) { // for stdlib qsort
    return _test_sorts_compare((void *)data1, (void *)data2);
}

static void test_sorts_dump_data(void *data, size_t n) {
    test_sorts_data_t *tsd = (test_sorts_data_t *)data;
    for (int i = 0; i < n; i++)
        printf("%" PRIu64 " ", tsd[i].tsd_key);
    //printf("\n");
}

static sorting_ops_t test_sorts_ops = {
    .compare_fn = _test_sorts_compare,
    .dump_data_fn = test_sorts_dump_data
};

static void verify(test_sorts_data_t *tsd, int n) {
    for (int i = 0; i < (n - 1); i++)
        assert(test_sorts_compare((void *)&tsd[i], (void *)&tsd[i + 1]) <= 0);
}

void do_stdlib_quick_sort(test_sorts_data_t *tsd, int n) {
    test_sorts_data_t *_tsd;
    struct timespec tstart, tend;
    double tm;
    
    assert(_tsd = malloc(sizeof(test_sorts_data_t) * n));
    memcpy(_tsd, tsd, sizeof(test_sorts_data_t) * n);
    
    //test_sorts_dump_data(_tsd, n);
    
    printf("stdlib quick sort ...");
    
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    qsort((void *)_tsd, n, sizeof(test_sorts_data_t), test_sorts_compare);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    tm = diff_timespec(tend, tstart);
    
    verify(_tsd, n);
    //test_sorts_dump_data(_tsd, n);
    
    printf("\rstdlib quick sort: %fs\n", tm);
    
    free(_tsd);
}

void do_quick_sort(test_sorts_data_t *tsd, int n) {
    test_sorts_data_t *_tsd;
    struct timespec tstart, tend;
    double tm;
    
    assert(_tsd = malloc(sizeof(test_sorts_data_t) * n));
    memcpy(_tsd, tsd, sizeof(test_sorts_data_t) * n);
    
    //test_sorts_dump_data(_tsd, n);
    
    printf("quick sort ...");
    
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    assert(quick_sort((void *)_tsd, sizeof(test_sorts_data_t), n, &test_sorts_ops) == 0);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    tm = diff_timespec(tend, tstart);
    
    verify(_tsd, n);
    //test_sorts_dump_data(_tsd, n);
    
    printf("\rquick sort: %fs\n", tm);
    
    free(_tsd);
}

void do_quick_sort_mt(test_sorts_data_t *tsd, int n) {
    test_sorts_data_t *_tsd;
    struct timespec tstart, tend;
    double tm;
    
    assert(_tsd = malloc(sizeof(test_sorts_data_t) * n));
    memcpy(_tsd, tsd, sizeof(test_sorts_data_t) * n);
    
    //test_sorts_dump_data(_tsd, n);
    
    printf("quick sort mt...");
    
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    assert(quick_sort_mt((void *)_tsd, sizeof(test_sorts_data_t), n, &test_sorts_ops) == 0);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    tm = diff_timespec(tend, tstart);
    
    verify(_tsd, n);
    //test_sorts_dump_data(_tsd, n);
    
    printf("\rquick sort mt: %fs\n", tm);
    
    free(_tsd);
}

void do_tree_sort(test_sorts_data_t *tsd, int n) {
    test_sorts_data_t *_tsd;
    struct timespec tstart, tend;
    double tm;
    
    assert(_tsd = malloc(sizeof(test_sorts_data_t) * n));
    memcpy(_tsd, tsd, sizeof(test_sorts_data_t) * n);
    
    //test_sorts_dump_data(_tsd, n);
    
    printf("tree sort ...");
    
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    assert(tree_sort((void *)_tsd, sizeof(test_sorts_data_t), n, &test_sorts_ops) == 0);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    tm = diff_timespec(tend, tstart);
    
    verify(_tsd, n);
    //test_sorts_dump_data(_tsd, n);
    
    printf("\rtree sort: %fs\n", tm);
    
    free(_tsd);
}

void do_heap_sort(test_sorts_data_t *tsd, int n) {
    test_sorts_data_t *_tsd;
    struct timespec tstart, tend;
    double tm;
    
    assert(_tsd = malloc(sizeof(test_sorts_data_t) * n));
    memcpy(_tsd, tsd, sizeof(test_sorts_data_t) * n);
    
    //test_sorts_dump_data(_tsd, n);
    
    printf("heap sort ...");
    
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    assert(heap_sort((void *)_tsd, sizeof(test_sorts_data_t), n, &test_sorts_ops) == 0);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    tm = diff_timespec(tend, tstart);
    
    verify(_tsd, n);
    //test_sorts_dump_data(_tsd, n);
    
    printf("\rheap sort: %fs\n", tm);
    
    free(_tsd);
}

void do_merge_sort(test_sorts_data_t *tsd, int n) {
    test_sorts_data_t *_tsd;
    struct timespec tstart, tend;
    double tm;
    
    assert(_tsd = malloc(sizeof(test_sorts_data_t) * n));
    memcpy(_tsd, tsd, sizeof(test_sorts_data_t) * n);
    
    //test_sorts_dump_data(_tsd, n);
    
    printf("merge sort ...");
    
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    assert(merge_sort((void *)_tsd, sizeof(test_sorts_data_t), n, &test_sorts_ops) == 0);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    tm = diff_timespec(tend, tstart);
    
    verify(_tsd, n);
    //test_sorts_dump_data(_tsd, n);
    
    printf("\rmerge sort: %fs\n", tm);
    
    free(_tsd);
}

void do_merge_sort_mt(test_sorts_data_t *tsd, int n) {
    test_sorts_data_t *_tsd;
    struct timespec tstart, tend;
    double tm;
    
    assert(_tsd = malloc(sizeof(test_sorts_data_t) * n));
    memcpy(_tsd, tsd, sizeof(test_sorts_data_t) * n);
    
    //test_sorts_dump_data(_tsd, n);
    
    printf("merge sort mt...");
    
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    assert(merge_sort_mt((void *)_tsd, sizeof(test_sorts_data_t), n, &test_sorts_ops) == 0);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    tm = diff_timespec(tend, tstart);
    
    verify(_tsd, n);
    //test_sorts_dump_data(_tsd, n);
    
    printf("\rmerge sort mt: %fs\n", tm);
    
    free(_tsd);
}

void do_insertion_sort(test_sorts_data_t *tsd, int n) {
    test_sorts_data_t *_tsd;
    struct timespec tstart, tend;
    double tm;
    
    assert(_tsd = malloc(sizeof(test_sorts_data_t) * n));
    memcpy(_tsd, tsd, sizeof(test_sorts_data_t) * n);
    
    //test_sorts_dump_data(_tsd, n);
    
    printf("insertion sort ...");
    
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    assert(insertion_sort((void *)_tsd, sizeof(test_sorts_data_t), n, &test_sorts_ops) == 0);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    tm = diff_timespec(tend, tstart);
    
    verify(_tsd, n);
    //test_sorts_dump_data(_tsd, n);
    
    printf("\rinsertion sort: %fs\n", tm);
    
    free(_tsd);
}


void do_selection_sort(test_sorts_data_t *tsd, int n) {
    test_sorts_data_t *_tsd;
    struct timespec tstart, tend;
    double tm;
    
    assert(_tsd = malloc(sizeof(test_sorts_data_t) * n));
    memcpy(_tsd, tsd, sizeof(test_sorts_data_t) * n);
    
    //test_sorts_dump_data(_tsd, n);
    
    printf("selection sort ...");
    
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    assert(selection_sort((void *)_tsd, sizeof(test_sorts_data_t), n, &test_sorts_ops) == 0);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    tm = diff_timespec(tend, tstart);
    
    verify(_tsd, n);
    //test_sorts_dump_data(_tsd, n);
    
    printf("\rselection sort: %fs\n", tm);
    
    free(_tsd);
}

static unsigned int test_sorts_hash(void *data) {
    test_sorts_data_t *tsd = (test_sorts_data_t *)data;
    return ht_default_hash((void *)tsd, sizeof(tsd->tsd_key));
}

void do_sorts(int n) {
    test_sorts_data_t *tsd;
    hash_table_t *ht;
    hash_table_ops_t hops;
    unsigned retries = 0;
    
    printf("building data set for sorts (%d elements)...\n", n);
    
    memset(&hops, 0, sizeof(hash_table_ops_t));
    hops.hto_hash_fn = test_sorts_hash;
    hops.hto_lops.llo_compare_fn = _test_sorts_compare;
    assert(ht = ht_create(&hops));
    
    assert(tsd = malloc(sizeof(test_sorts_data_t) * n));
    
    for (int i = 0; i < n; i++) {
        tsd[i].tsd_key = (((uint64_t)rand() << 32) | ((uint64_t)rand()));
        while (ht_find(ht, (void *)&tsd[i], NULL) == 0) { // make sure it's unique
            tsd[i].tsd_key = (((uint64_t)rand() << 32) | ((uint64_t)rand()));
            retries++;
        }
        ht_insert(ht, (void *)&tsd[i]);
    }
    ht_destroy(ht);
    //printf("retries %u\n", retries);
    
    do_stdlib_quick_sort(tsd, n); // n*logn
    do_merge_sort_mt(tsd, n); // n*logn
    do_merge_sort(tsd, n); // n*logn

    do_quick_sort_mt(tsd, n); // n*logn
    do_quick_sort(tsd, n); // n*logn
    //do_heap_sort(tsd, n); // n*logn
    //do_tree_sort(tsd, n); // n*logn
    //do_selection_sort(tsd, n); // n^2
    //do_insertion_sort(tsd, n); // n^2
    
    free(tsd);
}

#define DEFAULT_NUM_ELEMENTS (1 << 10)

int main (int argc, char **argv) {
    unsigned int seed = 0;
    int ch, num_elements = DEFAULT_NUM_ELEMENTS;
    
    
    struct option lonllopts[] = {
        { "num",    required_argument,   NULL,   'n' },
        { "seed",   required_argument,   NULL,   's' },
        { NULL,                0,        NULL,    0 }
    };

    while ((ch = getopt_long(argc, argv, "", lonllopts, NULL)) != -1) {
        switch (ch) {
            case 'n':
                num_elements = (int)strtol(optarg, NULL, 10);
                break;
            case 's':
                seed = (unsigned int)strtol(optarg, NULL, 10);
                break;
            default:
                printf("usage: %s [--num <num-elements>] [--seed <seed>]\n", argv[0]);
                return -1;
        }
    }
    
    if (!seed) {
        seed = (unsigned int)time(NULL);
        srand(seed);
    } else
        srand(seed);
    
    printf("seed %u\n", seed);
    
    setbuf(stdout, NULL); // don't buffer stdout
    
    do_sorts(num_elements);
    
    return 0;
}
