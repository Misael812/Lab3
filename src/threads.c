#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>

#include "thread_fn.h"

#define STACKSIZE 2000
#define SLEEPTIME 1000

struct k_thread coop_thread;
K_THREAD_STACK_DEFINE(coop_stack, STACKSIZE);

struct k_sem semaphore;
int counter;
void thread_entry(void)
{
	struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);

	while (1) {
        thread_counter(&counter, &semaphore, "thread",&timer, K_MSEC(SLEEPTIME));
	}
}

int main(void)
{
    counter = 0;
    k_sem_init(&semaphore, 1,1);
    k_thread_create(&coop_thread,
                    coop_stack,
                    STACKSIZE,
                    (k_thread_entry_t) thread_entry,
                    NULL,
                    NULL,
                    NULL,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);

	struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);

	while (1) {
        thread_counter(&counter, &semaphore, "main",&timer, K_MSEC(SLEEPTIME));
        // k_sem_take(&semaphore, K_FOREVER);
        // main_thread(&counter);
        // k_sem_give(&semaphore);
		// k_timer_start(&timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
		// k_timer_status_sync(&timer);
	}

	return 0;
}
