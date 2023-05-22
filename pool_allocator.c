#include "pool_allocator.h"

#if _MT_TYPE == MT_POSIX
pthread_mutex_t pool_mutex = PTHREAD_MUTEX_INITIALIZER;
void pool_mutex_lock() {
    pthread_mutex_lock(&pool_mutex);
}

void pool_mutex_unlock() {
    pthread_mutex_unlock(&pool_mutex);
}
#endif

#if _MT_TYPE == MT_CUSTOM
// Place your variables here
void pool_mutex_lock() {
    // Write your own Mutex Lock implementation here
}

void pool_mutex_unlock() {
    // Write your own Mutex Unlock implementation here
}
#endif


#if _MEMORY_TYPE == GLOBAL
uint8_t memory_pool[BLOCK_NUMBER * BLOCK_SIZE];
Pool pool_alloc() {
    Pool p;
    p.block_number = BLOCK_NUMBER;
    p.block_size = BLOCK_SIZE;
    pool_mutex_lock();
    p.block_start = (Block*)memory_pool;
    
    Block *b = p.block_start;

    // Using pointer arithmetics to create linked list of memory blocks

    for (uint16_t i = 0; i < BLOCK_NUMBER; i++)
    {
        uint8_t* tmp = (uint8_t*)b;
        tmp += BLOCK_SIZE;
        b->next = (Block *)tmp;
        b = b->next;
    } 
    b->next = NULL;
    pool_mutex_unlock();
    return p;
}
#endif

#if _MEMORY_TYPE == HEAP
Pool pool_alloc() {
    Pool p;
    p.block_number = BLOCK_NUMBER;
    p.block_size = BLOCK_SIZE;
    pool_mutex_lock();
    p.block_start = malloc(BLOCK_NUMBER * BLOCK_SIZE * sizeof(uint8_t));
    if (p.block_start == NULL) {
        return p;
    }
    Block *b = p.block_start;
    for (uint16_t i = 0; i < BLOCK_NUMBER; i++)
    {
        uint8_t* tmp = (uint8_t*)b;
        tmp += BLOCK_SIZE;
        b->next = (Block *)tmp;
        b = b->next;
    } 
    b->next = NULL;
    pool_mutex_unlock();
    return p;
}
#endif

Block* block_alloc(Pool* p) {
    pool_mutex_lock();
    if (p->block_start == NULL) {
        return NULL;
    }

    //Returning first element of linked list, then assigning list start to next segment

    Block* b = p->block_start;
    p->block_start = b->next;
    pool_mutex_unlock();
    return b;
}

void block_dealloc(Pool* p, Block* b) {
    pool_mutex_lock();

    //Simply linking deallocated element to start of pool, making it free to use again
    //User need to explicitly assign NULL to pointers to deallocated block

    b->next = p->block_start;
    p->block_start = b;
    pool_mutex_unlock();
}






