#ifndef HEAP_ALLOC_H
#define HEAP_ALLOC_H

#include <stddef.h>

void *heap_alloc(size_t size);
void heap_free(void *ptr);

#endif // !HEAP_ALLOC_H
