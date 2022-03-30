#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

void swap(void *a, void *b, size_t sz) {
    uint8_t tmp[sz];
    memcpy(tmp, a, sz);
    memcpy(a, b, sz);
    memcpy(b, tmp, sz);
}
