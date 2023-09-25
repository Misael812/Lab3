#include "thread_fn.h"

# define TIMER_TIME 1000
# define STACKSIZE 2000
//I think we need to rework these, so that the semaphore take and give is within the function.
// Shown below in the thread_counter function.

/**
 * @file thread_fn.c
 * @brief This file contains the implementation of a function that increments a counter and prints a message with the thread name and the current count.
 */

/**
 * @brief Function that increments a counter and prints a message with the thread name and the current count.
 * 
 * @param counter Pointer to an integer that represents the counter to be incremented.
 * @param semaphore Semaphore used to synchronize access to the counter.
 * @param thread_name String that represents the name of the thread.
 * @param thread_priority Integer that represents the priority of the thread.
 */
void thread_counter(int* counter,
                    struct k_sem *semaphore,
                    char* thread_name,
                    struct k_timer *timer,
                    k_timeout_t timeout){

    if (k_sem_take(semaphore, timeout) == 1){
        *counter = *counter + 1;
        printk("hello world from %s! Count %d\n", thread_name, *counter);
        k_sem_give(semaphore);
        return;
    }
    else{
        k_sem_give(semaphore);
        k_timer_start(timer, K_MSEC(TIMER_TIME), K_NO_WAIT);
        k_timer_status_sync(timer);
        return;
    }
}

/**
 * The main_thread function increments the value of the counter pointer by 1 and prints a message to the kernel log with the current count value.
 *
 * @param counter A pointer to an integer representing the current count value.
 */
void main_thread(int* counter){

    *counter = *counter + 1;
	printk("hello world from %s! Count %d\n", "main", *counter);
}

/**
 * This function increments the value of a counter and prints a message to the kernel log.
 *
 * @param counter A pointer to an integer representing the counter to be incremented.
 */
void thread_thread(int* counter){
    *counter = *counter + 1;
    printk("hello world from %s! Count %d\n", "thread", *counter);
}