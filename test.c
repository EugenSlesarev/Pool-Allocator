#include "unity.h"
#include "pool_allocator.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

struct args {
    Pool* pool;
    uint8_t value;
};

void *just_some_adding(void *ptr);
void *test_dealloc(void *ptr);


void test_allocating_all_pool(void) {
    Pool pool = pool_alloc();
    if (pool.block_start == NULL) {
        TEST_FAIL_MESSAGE("Pool not allocated");
        return;
    }
    uint8_t* a[BLOCK_NUMBER];
    uint32_t sum = 0;
    for (uint16_t i = 0; i < BLOCK_NUMBER; i++)
    {
        a[i] = (uint8_t*)block_alloc(&pool);
        if(a[i] == NULL) {
            TEST_FAIL_MESSAGE("Block not allocated");
            return;
        }
        for (uint16_t j = 0; j < BLOCK_SIZE; j++)
        {
            a[i][j] = 1;
            sum+=a[i][j];
        }
        
    }
    TEST_ASSERT_EQUAL(BLOCK_NUMBER*BLOCK_SIZE, sum);
    
}

void test_two_threads(void) {
    pthread_t thread1, thread2;
    int  iret1, iret2;
    Pool pool = pool_alloc();
    if (pool.block_start == NULL) {
        TEST_FAIL_MESSAGE("Pool not allocated");
        return;
    }
    struct args a1 = {&pool, 2};
    struct args a2 = {&pool, 3};
    iret1 = pthread_create( &thread1, NULL, just_some_adding, (void*)&a1);
    iret2 = pthread_create( &thread2, NULL, just_some_adding, (void*)&a2);
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL); 
}


void *just_some_adding(void *ptr) {
    uint8_t* numbers[BLOCK_NUMBER/2];
    uint32_t sum = 0;
    struct args a = *(struct args*) ptr;
    Pool * pool;
    pool = a.pool;
    for (uint16_t i = 0; i < BLOCK_NUMBER/2; i++)
    {
        numbers[i] = (uint8_t*)block_alloc(pool);
        if(numbers[i] == NULL) {
            TEST_FAIL_MESSAGE("Block not allocated");
            return NULL;
        }
        for (uint16_t j = 0; j < BLOCK_SIZE; j++)
        {
            numbers[i][j] = a.value;
            sum+=numbers[i][j];
        }
        
    }
    TEST_ASSERT_EQUAL(BLOCK_NUMBER/2*BLOCK_SIZE*a.value, sum);
}

void test_two_threads_with_block_dealloc(void) {
    pthread_t thread1, thread2;
    int  iret1, iret2;
    Pool pool = pool_alloc();
    if (pool.block_start == NULL) {
        TEST_FAIL_MESSAGE("Pool not allocated");
        return;
    }
    struct args a1 = {&pool, 2};
    struct args a2 = {&pool, 4};
    iret1 = pthread_create( &thread1, NULL, test_dealloc, (void*)&a1);
    iret2 = pthread_create( &thread2, NULL, test_dealloc, (void*)&a2);
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL); 
}

void *test_dealloc(void *ptr) {
    uint8_t* x, * y, * z;
    uint32_t sum = 0;
    struct args a = *(struct args*) ptr;
    Pool * pool;
    pool = a.pool;
    x = (uint8_t*)block_alloc(pool);
    if(x == NULL) {
        TEST_FAIL_MESSAGE("Block not allocated");
        return NULL;
    }
    y = (uint8_t*)block_alloc(pool);
    if(y == NULL) {
        TEST_FAIL_MESSAGE("Block not allocated");
        return NULL;
    }
    z = (uint8_t*)block_alloc(pool);
    if(z == NULL) {
        TEST_FAIL_MESSAGE("Block not allocated");
        return NULL;
    }
    block_dealloc(pool, (Block*) y);
    y = NULL;
    for (uint16_t i = 0; i < BLOCK_SIZE; i++)
    {
        x[i] = a.value;
        z[i] = (a.value+1);
        sum+=x[i];
        sum+=z[i];
    }
    
    TEST_ASSERT_EQUAL(BLOCK_SIZE*a.value + BLOCK_SIZE*(a.value+1), sum);
}

int main(void) {
    UNITY_BEGIN();

    //Can we use all pool
    RUN_TEST(test_allocating_all_pool);
    for (int i = 0; i < 2000; i++)
    {
        //Can we use pool in two different threads
        RUN_TEST(test_two_threads);
        //Can we use pool in two different threads with deallocating some pool elements
        RUN_TEST(test_two_threads_with_block_dealloc);
    }
    return UNITY_END();
}