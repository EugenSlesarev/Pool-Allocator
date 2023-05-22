#ifndef POOL_CONF_H
#define POOL_CONF_H
/*
    This is configuration file for Pool Allocator library. It's providing default values, however it's recommended to
    check all settings before using.
*/

/*
    BLOCK_NUMBER defines how many chunks of memory will be allocated in pool
*/
#define BLOCK_NUMBER 10000
/*
    BLOCK_SIZE defines size of one chunk of memory in bytes
*/
#define BLOCK_SIZE 100
/*
    _MEMORY_TYPE defines type of memory used to allocate pool
*/
#define GLOBAL 0
#define HEAP 1
#define _MEMORY_TYPE GLOBAL
/*
    _MT_TYP defines multitasking implentation used
    In order to use custom implentation you need to manually implement mutex lock and unlock functions in pool_allocator.c
*/
#define MT_POSIX 0
#define MT_CUSTOM 1
#define _MT_TYPE MT_POSIX


#endif