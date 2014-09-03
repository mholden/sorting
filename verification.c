#include "include/verification.h"

/*
 * Verification function.
 *
 * Returns 0 if array correctly sorted, -1 otherwise.
 */

int verify(int *array, size_t n){
	int i;

	i=0;
	while(i < n - 1){
		if(array[i] > array[i + 1]) return -1;
		i++;
	}
	
	return 0;
}
