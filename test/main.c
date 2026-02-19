#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "heap_alloc.h"

#define STRESS_COUNT 1000
#define MAX_STRESS_SIZE 512

// --------------------------------------------
// Simple Test Runner
// --------------------------------------------

#define RUN_TEST(fn)                                                           \
    do                                                                         \
    {                                                                          \
        printf("Running %-35s", #fn);                                          \
        fflush(stdout);                                                        \
        fn();                                                                  \
        printf(" [OK]\n");                                                     \
    } while (0)

// --------------------------------------------
// Tests
// --------------------------------------------

void test_basic_alloc_free()
{
    void *ptr = heap_alloc(32);
    assert(ptr != NULL);
    heap_free(ptr);
}

void test_multiple_allocations()
{
    void *a = heap_alloc(16);
    void *b = heap_alloc(32);
    void *c = heap_alloc(64);

    assert(a && b && c);
    assert(a != b);
    assert(b != c);
    assert(a != c);

    heap_free(b);
    heap_free(a);
    heap_free(c);
}

void test_memory_write()
{
    size_t size = 128;
    void *ptr = heap_alloc(size);
    assert(ptr != NULL);

    memset(ptr, 0xAB, size);

    unsigned char *p = ptr;
    for (size_t i = 0; i < size; i++)
    {
        assert(p[i] == 0xAB);
    }

    heap_free(ptr);
}

void test_reuse_block()
{
    void *a = heap_alloc(128);
    assert(a != NULL);

    heap_free(a);

    void *b = heap_alloc(128);
    assert(b != NULL);

    // Many allocators reuse the same block.
    // If yours doesn't, remove this assertion.
    assert(a == b);

    heap_free(b);
}

void test_alignment()
{
    void *ptr = heap_alloc(24);
    assert(ptr != NULL);

    uintptr_t addr = (uintptr_t)ptr;

    // Most allocators align to pointer size
    assert(addr % sizeof(void *) == 0);

    heap_free(ptr);
}

void test_zero_alloc()
{
    void *ptr = heap_alloc(0);

    // Accept either behavior depending on implementation:
    // returning NULL or a valid pointer.
    if (ptr)
        heap_free(ptr);
}

void test_free_null()
{
    heap_free(NULL); // Must not crash
}

void test_fragmentation_pattern()
{
    void *a = heap_alloc(200);
    void *b = heap_alloc(200);
    void *c = heap_alloc(200);

    assert(a && b && c);

    heap_free(b);

    void *d = heap_alloc(180);
    assert(d != NULL);

    heap_free(a);
    heap_free(c);
    heap_free(d);
}

void test_stress()
{
    void *ptrs[STRESS_COUNT];

    for (int i = 0; i < STRESS_COUNT; i++)
    {
        size_t size = (rand() % MAX_STRESS_SIZE) + 1;
        ptrs[i] = heap_alloc(size);
        assert(ptrs[i] != NULL);
        memset(ptrs[i], 0xCD, size);
    }

    for (int i = 0; i < STRESS_COUNT; i++)
    {
        heap_free(ptrs[i]);
    }
}

// --------------------------------------------
// Main
// --------------------------------------------

int main(void)
{
    printf("===== Heap Allocator Test Suite =====\n\n");

    srand((unsigned int)time(NULL));

    RUN_TEST(test_basic_alloc_free);
    RUN_TEST(test_multiple_allocations);
    RUN_TEST(test_memory_write);
    RUN_TEST(test_reuse_block);
    RUN_TEST(test_alignment);
    RUN_TEST(test_zero_alloc);
    RUN_TEST(test_free_null);
    RUN_TEST(test_fragmentation_pattern);
    RUN_TEST(test_stress);

    printf("\nAll tests passed successfully 🎉\n");
    return 0;
}
