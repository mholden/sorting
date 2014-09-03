#include "include/swap.h"

void swap(int *a, int *b){
        int tmp;

        tmp = *b;
        *b = *a;
        *a = tmp;

        return;
}
