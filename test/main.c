#include "heap_alloc.h"
#include <stdio.h>

int main(void)
{
    int *a = heap_alloc(5 * sizeof(int));
    int *b = heap_alloc(5 * sizeof(int));

    heap_free(a);

    int *c = heap_alloc(5 * sizeof(int));

    printf("a: %p\n", a);
    printf("b: %p\n", b);
    printf("c: %p\n", c);

    return 0;
}
