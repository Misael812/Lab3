#include "thread_fn.h"
#include <zephyr.h>
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