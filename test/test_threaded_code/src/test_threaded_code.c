#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <stdio.h>


# define TIMEOUT_VAL 50

void setUp(void) {}

void tearDown(void) {}


void test_thread_thread_only(void, *semaphore) {
    int counter = 0;
    main_thread(&counter);
    thread_thread(&counter);
    TEST_ASSERT_EQUAL_INT(2, counter);
}

void test_main_thread_only(void, *semaphore) {
    int counter = 0;
    main_thread(&counter);
    // Test functionality within thread_thread
    TEST_ASSERT_EQUAL_INT(1, counter);
}


void test_main_thread_and_thread_thread(void, *semaphore) {
    int counter = 0;
    main_thread(&counter);
    thread_thread(&counter);
    main_thread(&counter);
    TEST_ASSERT_EQUAL_INT(3, counter);
}


void main(void){
    UNITY_BEGIN();
    struct k_sem semaphore;
    RUN_TEST(test_thread_thread_only, &semaphore);
    RUN_TEST(test_main_thread_only, &semaphore);
    RUN_TEST(test_main_thread_and_thread_thread, &semaphore);
    UNITY_END();
}
