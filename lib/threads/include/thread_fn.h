#pragma once
#include <zephyr.h>


void thread_counter(int* counter,
                    struct k_sem * semaphore,
                    char* thread_name,
                    struct k_timer *timer,
                    k_timeout_t timeout);
void deadlock_counter(int* counter,
                    struct k_sem * semaphore1,
                    struct k_sem * semaphore2);
void main_thread(int* counter);
void thread_thread(int* counter);
void orphaned_lock(struct k_sem * semaphore, k_timeout_t timeout, int * counter);
void unorphaned_lock(struct k_sem * semaphore, k_timeout_t timeout, int * counter);
