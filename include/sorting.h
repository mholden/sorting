#ifndef _SORTING_H_
#define _SORTING_H_

#include<stdlib.h>

/*
 * Definitions for our sorts.
 */

#define TEST_NAME_LEN 20

void bubble_sort(int *array, size_t n);
void insertion_sort(int *array, size_t n);
void selection_sort(int *array, size_t n);
void heap_sort(int *array, size_t n);
void merge_sort(int *array, size_t n);
void quick_sort(int *array, size_t n);
void hash_sort(int *array, size_t n);

#endif /* _SORTING_H_ */
