#include "pool_allocator.h"
#include <stdio.h>
int main(int) {
    Pool pool = pool_alloc();
    uint8_t* a = (uint8_t*)block_alloc(&pool);
    for (int i = 0; i < 10; i++)
    {
        a[i] = i*10;
        printf("%d", a[i]);
    }
    printf("\n");
    block_dealloc(&pool, (Block*)a);
    uint16_t* b = (uint16_t*)block_alloc(&pool);
    for (int i = 0; i < 5; i++)
    {
        b[i] = i+1;
        printf("%d", b[i]);
    }
    





    return 0;
}