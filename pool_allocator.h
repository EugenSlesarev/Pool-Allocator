#ifndef POOL_H
#define POOL_H

#include <stdint.h>
#include "pool_allocator_conf.h"

#ifdef MT_POSIX
#include <pthread.h>
#endif

#ifdef MEMORY_HEAP
#include <stdlib.h> 
#endif

typedef struct Block Block;
struct Block{
    Block* next;
};

typedef struct{
    uint16_t block_number;
    uint16_t block_size;
    Block* block_start;
} Pool;

Pool pool_alloc();
Block* block_alloc(Pool* p);
void block_dealloc(Pool* p, Block* b);
void pool_mutex_lock();
void pool_mutex_unlock();


#endif