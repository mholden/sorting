#include "include/sorting.h"
#include "include/verification.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*
 * Main - run all sorts and report performance.
 */

int main(int argc, char **argv){
	int n, i;
	int *array, *array_cp;
	clock_t start, end;
	double tot_time;
	char test_name[TEST_NAME_LEN];

	if(argc != 2){
		printf("Incorrect argument count. "
		       "Usage: ./int_sort array_size\n");

		return -1;
	}
	
	/* Generate our array(s) */
	n = atoi(argv[1]);
	array = malloc(n * sizeof(int));
	if(array == NULL){
		printf("Out of memory?\n");
		return -1;
	}
	array_cp = malloc(n * sizeof(int));
	if(array_cp == NULL){
		printf("Out of memory?\n");
		return -1;
	}	

	for(i=0; i < n; i++){
		array[i] = rand() % (2*n);
	}

	/* Run the sorts, verify results and report performances */
	printf("Test\t\tArray Size\tSort Time\n");

	/* 1. Bubble Sort */
	strcpy(test_name, "bubble_sort");
	memcpy((void *)array_cp, (void *)array, (n * sizeof(int)));

	start = clock();
	bubble_sort(array_cp, n);
	end = clock();	

	tot_time = (double) (end - start) / CLOCKS_PER_SEC;

	if(!verify(array_cp, n)){
		printf("%s\t%d\t\t%fs\n", test_name, n, tot_time);	
	}
	else{
		printf("Sorting Error: %s\n", test_name);
		free(array);
		free(array_cp);
		return -1;
	}

	/* 2. Insertion Sort */
	strcpy(test_name, "insertion_sort");
        memcpy((void *)array_cp, (void *)array, (n * sizeof(int)));

        start = clock();
        insertion_sort(array_cp, n);
        end = clock();

        tot_time = (double) (end - start) / CLOCKS_PER_SEC;

        if(!verify(array_cp, n)){
                printf("%s\t%d\t\t%fs\n", test_name, n, tot_time);
        }
        else{
                printf("Sorting Error: %s\n", test_name);
                free(array);
                free(array_cp);
                return -1;
        }	

	/* 3. Selection Sort */
	strcpy(test_name, "selection_sort");
        memcpy((void *)array_cp, (void *)array, (n * sizeof(int)));

        start = clock();
        selection_sort(array_cp, n);
        end = clock();

        tot_time = (double) (end - start) / CLOCKS_PER_SEC;

        if(!verify(array_cp, n)){
                printf("%s\t%d\t\t%fs\n", test_name, n, tot_time);
        }
        else{
                printf("Sorting Error: %s\n", test_name);
                free(array);
                free(array_cp);
                return -1;
        }

	/* 4. Heap Sort */
	strcpy(test_name, "heap_sort");
        memcpy((void *)array_cp, (void *)array, (n * sizeof(int)));

        start = clock();
        heap_sort(array_cp, n);
        end = clock();

        tot_time = (double) (end - start) / CLOCKS_PER_SEC;

        if(!verify(array_cp, n)){
                printf("%s\t%d\t\t%fs\n", test_name, n, tot_time);
        }
        else{
                printf("Sorting Error: %s\n", test_name);
                free(array);
                free(array_cp);
                return -1;
        }

	/* 5. Merge Sort */
	strcpy(test_name, "merge_sort");
        memcpy((void *)array_cp, (void *)array, (n * sizeof(int)));

        start = clock();
        merge_sort(array_cp, n);
        end = clock();

        tot_time = (double) (end - start) / CLOCKS_PER_SEC;

        if(!verify(array_cp, n)){
                printf("%s\t%d\t\t%fs\n", test_name, n, tot_time);
        }
        else{
                printf("Sorting Error: %s\n", test_name);
                free(array);
                free(array_cp);
                return -1;
        }
	
	/* 6. Quick Sort */
	strcpy(test_name, "quick_sort");
        memcpy((void *)array_cp, (void *)array, (n * sizeof(int)));

        start = clock();
        quick_sort(array_cp, n);
        end = clock();

        tot_time = (double) (end - start) / CLOCKS_PER_SEC;

        if(!verify(array_cp, n)){
                printf("%s\t%d\t\t%fs\n", test_name, n, tot_time);
        }
        else{
                printf("Sorting Error: %s\n", test_name);
                free(array);
                free(array_cp);
                return -1;
        }	

	/* 7. Hash Sort */
	strcpy(test_name, "hash_sort");
        memcpy((void *)array_cp, (void *)array, (n * sizeof(int)));

        start = clock();
        hash_sort(array_cp, n);
        end = clock();

        tot_time = (double) (end - start) / CLOCKS_PER_SEC;

        if(!verify(array_cp, n)){
                printf("%s\t%d\t\t%fs\n", test_name, n, tot_time);
        }
        else{
                printf("Sorting Error: %s\n", test_name);
                free(array);
                free(array_cp);
                return -1;
        }

	/* Clean up and return */
	free(array);
	free(array_cp);

	return 0;
}
