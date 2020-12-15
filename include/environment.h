#pragma once

#if __STDC_HOSTED__ == 1
#include <stdlib.h>

inline void *allocate(size_t size) {
    return malloc(size);
}

inline void *reallocate(void *pointer, size_t size) {
    return realloc(pointer, size);
}

inline void deallocate(void *pointer) {
    return free(pointer);
}
#else
void *allocate(size_t size);

void *reallocate(void *pointer, size_t new_size);

void deallocate(void *pointer);
#endif