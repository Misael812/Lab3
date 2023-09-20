#include "thread_fn.h"
#include <zephyr.h>
void main_thread(int* counter){
    *counter = *counter + 1;
	printk("hello world from %s! Count %d\n", "main", *counter);
}

void thread_thread(int* counter){
    *counter = *counter + 1;
    printk("hello world from %s! Count %d\n", "thread", *counter);
}