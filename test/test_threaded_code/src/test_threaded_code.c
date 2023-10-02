#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <stdio.h>
#include <unity.h>
#include "thread_fn.h"


# define TIMEOUT_VAL 50


struct k_sem semaphore;
struct k_timer timer;

struct k_sem semaphore1;
struct k_sem semaphore2;


void setUp(void) {
    k_sem_init(&semaphore, 1,1);
    k_timer_init(&timer, NULL, NULL);

    k_sem_init(&semaphore1, 1, 1);
    k_sem_init(&semaphore2, 1, 1);
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

/// Activity 4 ///
#define STACK_SIZE 500
K_THREAD_STACK_DEFINE(stack1, STACK_SIZE);
K_THREAD_STACK_DEFINE(stack2, STACK_SIZE);
K_THREAD_STACK_DEFINE(supervisor_stack, STACK_SIZE);
struct k_thread thread1, thread2, supervisor_thread;
int deadlock_counter_variable = 0;

void deadlock_supervisor(void){
    //Create threads to deadlock
    k_tid_t t1 = k_thread_create(&thread1, stack1, STACK_SIZE,
                                    (k_thread_entry_t)deadlock_counter,
                                    &deadlock_counter_variable, &semaphore1, &semaphore2,
                                    K_PRIO_COOP(6),
                                    0,
                                    K_NO_WAIT);

    k_tid_t t2 = k_thread_create(&thread2, stack2, STACK_SIZE,
                                    (k_thread_entry_t)deadlock_counter,
                                    &deadlock_counter_variable, &semaphore2, &semaphore1,
                                    K_PRIO_COOP(6),
                                    0,
                                    K_NO_WAIT);

    int t1_done = k_thread_join(t1, K_MSEC(1000));
    int t2_done = k_thread_join(t2, K_MSEC(1000));

    TEST_ASSERT_EQUAL_INT(-EAGAIN, t1_done);
    TEST_ASSERT_EQUAL_INT(-EAGAIN, t2_done);

    k_thread_abort(t1);
    k_thread_abort(t2);
}

void test_deadlock(void){
    k_thread_create(&supervisor_thread, supervisor_stack, STACK_SIZE,
                    (k_thread_entry_t) deadlock_supervisor,
                    NULL, NULL, NULL,
                    K_PRIO_COOP(7),
                    0, K_NO_WAIT);

    k_thread_join(&supervisor_thread, K_FOREVER);

    // The incrementing in our deadlock function happens
    // after the section that deadlocks so we should expect
    // the counter to stay at zero
    TEST_ASSERT_EQUAL_INT(0, deadlock_counter_variable);
}

void test_orphanlock(void){
    struct k_sem semaphore_orphan;
    k_sem_init(&semaphore_orphan, 1, 1);

    int count = 1;

    orphaned_lock(&semaphore_orphan, K_MSEC(500), &count);

    TEST_ASSERT_EQUAL_INT(2, count);
    TEST_ASSERT_EQUAL_INT(1, k_sem_count_get(&semaphore_orphan));

    orphaned_lock(&semaphore_orphan, K_MSEC(500), &count);

    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_INT(0, k_sem_count_get(&semaphore_orphan));

    orphaned_lock(&semaphore_orphan, K_MSEC(500), &count);

    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_INT(0, k_sem_count_get(&semaphore_orphan));
}

void test_unorphanlock(void){
    struct k_sem semaphore_orphan;
    k_sem_init(&semaphore_orphan, 1, 1);

    int count = 1;

    unorphaned_lock(&semaphore_orphan, K_MSEC(500), &count);

    TEST_ASSERT_EQUAL_INT(2, count);
    TEST_ASSERT_EQUAL_INT(1, k_sem_count_get(&semaphore_orphan));

    unorphaned_lock(&semaphore_orphan, K_MSEC(500), &count);

    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_INT(1, k_sem_count_get(&semaphore_orphan));

    unorphaned_lock(&semaphore_orphan, K_MSEC(500), &count);

    TEST_ASSERT_EQUAL_INT(4, count);
    TEST_ASSERT_EQUAL_INT(1, k_sem_count_get(&semaphore_orphan));
}


void main(void){
    UNITY_BEGIN();
    RUN_TEST(test_locks_thread_counter);
    RUN_TEST(test_deadlock);
    RUN_TEST(test_orphanlock);
    RUN_TEST(test_unorphanlock);
    UNITY_END();
}
