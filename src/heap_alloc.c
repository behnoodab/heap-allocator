#include "internal.h"
#include <sys/mman.h>
#include <unistd.h>

static block_meta_t *global_base = NULL;

static block_meta_t *request_space(size_t size)
{
    size_t total_size = size + META_SIZE;

    block_meta_t *block = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (block == MAP_FAILED)
    {
        return NULL;
    }

    block->size = size;
    block->free = 0;
    block->next = NULL;

    return block;
}

static block_meta_t *find_free_block(size_t size)
{
    block_meta_t *current = global_base;

    while (current)
    {
        if (current->free && current->size >= size)
            return current;
        current = current->next;
    }

    return NULL;
}

void *heap_alloc(size_t size)
{
    if (size == 0)
        return NULL;

    block_meta_t *block;

    if (!global_base)
    {
        block = request_space(size);
        if (!block)
            return NULL;
        global_base = block;
    }
    else
    {
        block = find_free_block(size);

        if (!block)
        {
            block = request_space(size);
            if (!block)
                return NULL;

            block_meta_t *last = global_base;
            while (last->next)
                last = last->next;

            last->next = block;
        }
        else
        {
            block->free = 0;
        }
    }

    return (block + 1);
}

void heap_free(void *ptr)
{
    if (!ptr)
        return;

    block_meta_t *block = (block_meta_t *)ptr - 1;
    block->free = 1;
}
