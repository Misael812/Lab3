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
* The purpose of this function is to be called as an entry point for two different threads. These threads should reference the
* same semaphores but pass in opposite of each other (thread one get semaphore a then sema b, thread two get semaphore b then a).
* This should cause the two threads to deadlock where both threads are waiting for each other's semaphore. To ensure this behavior
* is consistent k_yield() is called to allow another thread to take action, making sure both semaphores/locks are taken before
* moving on to deadlock
*
* @param counter pointer to integer, to be increment by threads. Not too important here
* @param semaphore1 the first lock/semaphore to be passed the deadlock (pointer to struct k_sem)
* @param semaphore2 the second lock/semaphore to be passed the deadlock (pointer to struct k_sem)
*/
void deadlock_counter(int* counter,
                    struct k_sem * semaphore1,
                    struct k_sem * semaphore2){

    struct k_timer timer;
    k_timer_init(&timer, NULL, NULL);
    k_sem_take(semaphore1, K_FOREVER);{
        k_yield();
        k_timer_start(&timer, K_MSEC(100), K_NO_WAIT);
        k_timer_status_sync(&timer);
        k_sem_take(semaphore2, K_FOREVER);
        {
            *counter = *counter + 1;
            printk("hello world from %s! Count %d\n", *counter);
        }
        k_sem_give(semaphore2);
        k_sem_give(semaphore1);
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