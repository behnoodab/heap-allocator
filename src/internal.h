#ifndef INTERNAL_H
#define INTERNAL_H

#include <stddef.h>

typedef struct block_meta
{
    size_t size;
    int free;
    struct block_meta *next;
} block_meta_t;

#define META_SIZE sizeof(block_meta_t)

#endif // !INTERNAL_H
