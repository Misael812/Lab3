#pragma once
#include <zephyr.h>


void thread_counter(int* counter,
                    struct k_sem * semaphore,
                    char* thread_name,
                    struct k_timer *timer,
                    k_timeout_t timeout);
void main_thread(int* counter);
void thread_thread(int* counter);