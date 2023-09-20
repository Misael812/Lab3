### Activity 0

1. ~~Create a new project.~~
2. ~~Copy in the `thread.c` example file. Commit.~~
3. ~~Identify the execution contexts in the program, and their entry points.~~
4. ~~Identify shared state between execution contexts.~~
5. ~~Identify the semaphore.~~
6. ~~Predict the behavior of the program.~~
7. ~~Run the program and compare the output to your prediction.~~

### [](https://github.com/uofu-emb/rtos/tree/main/labs/3.threads#activity-1)Activity 1

1. Are all uses of the shared resources in protected critical sections? Make any modifications necessary to protect the critical sections.
2. Is the critical section starving the system? If so, make modifications to prevent starving the system.

### Activity 2

1. Write a unit test for the thread code. Remember to separate functionality from execution context concerns, timing, and looping.
2. You'll need to make some modifications to the code. Hints:
    1. You don't need to test execution in a thread, but you should test the behavior of the lock and the side effect.
    2. k_semaphore_take includes a timeout on waiting for the semaphore.
    3. k_semaphore_take returns a status code, don't forget to check it.
3. Don't forget to commit as you go.

### Activity 3

1. Create a thread that calculates the nth element of the Fibonacci sequence.
    1. Fibonacci sequence defines the nth element as the sum of the n-1 and n-2 elements. n=0 is 0 and n=1 is 1.
    2. You don't need to store the whole sequence, just calculate the target element.
    3. You'll need to use entry point parameters on k_thread_create.
    4. Pass in a target element number and an output parameter to store the result.
2. Create a supervisor thread implementation that spawns 16 copies of the thread to calculate 16 elements, then blocks waiting for all threads to exit before exiting.
    1. Hint: Look at k_thread_join, K_THREAD_STACK_ARRAY_DEFINE.
3. Write a test. Your test should spawn the supervisor thread and wait for it to complete and check the results.


### Activity 4

1. Write code that creates this situation.
2. Write a test that shows this code will lock.
3. [https://docs.zephyrproject.org/2.7.5/reference/timing_functions/index.html](https://docs.zephyrproject.org/2.7.5/reference/timing_functions/index.html)



### Activity 5

1. Write a test for the functionality of the thread.
2. Write a test showing the thread will deadlock.
3. Create a new version of the code that will not deadlock.
4. Write a test showing the thread will not deadlock.