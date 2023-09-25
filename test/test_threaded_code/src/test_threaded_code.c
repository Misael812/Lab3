#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <stdio.h>
#include <unity.h>
#include "thread_fn.h"


# define TIMEOUT_VAL 50


struct k_sem semaphore;
struct k_timer timer;

void setUp(void) {
    k_sem_init(&semaphore, 1,1);
    k_timer_init(&timer, NULL, NULL);
}

void tearDown(void) {}

/// Activity 2 ///
void test_locks_thread_counter() {
    int counter = 0;

    // Test if the semaphore locks the thread_counter function
    k_sem_take(&semaphore, K_FOREVER);
    thread_counter(&counter, &semaphore, "thread", &timer, K_MSEC(100));
    
    // Tests to make sure that the counter is not incremented
    TEST_ASSERT_EQUAL_INT(0, counter);
}



void main(void){
    UNITY_BEGIN();
    RUN_TEST(test_locks_thread_counter);
    UNITY_END();
}
