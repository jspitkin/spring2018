/* author: jake pitkin
 * last edit: april 2 2018
 * assignment 4 - problem 5
 * cs5460 - operating systems
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sched.h>

typedef struct __ret_t {
    int cs_count;
} ret_t;

typedef struct __spin_lock_t {
    volatile int counter;
    volatile int turn;
} spin_lock_t;

volatile int in_cs;
volatile spin_lock_t *lock;
long thread_count;
long seconds;
volatile int run_threads;

void spin_lock(volatile spin_lock_t *s);
void spin_unlock(volatile spin_lock_t *s);
void *routine();

int main(int argc, char *argv[]) {
    // Validate input
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments.\n");
        return -1;
    }
    thread_count = strtol(argv[1], NULL, 10);
    seconds = strtol(argv[2], NULL, 10);
    run_threads = 1;
    if (thread_count < 1 || thread_count > 99) {
        fprintf(stderr, "Invalid input for threads. Expected [1, 99].\n");
	return -1;
    }
    if (seconds < 1) {
        fprintf(stderr, "Invalid input for seconds. Expected positive integer.\n");
        return -1;
    }

    // Allocate threads and lock
    pthread_t *threads = (pthread_t*) malloc(thread_count*sizeof(pthread_t));
    lock = (spin_lock_t*) malloc(sizeof(spin_lock_t));
    if (threads == NULL || lock == NULL) {
	fprintf(stderr, "Error allocating memory.\n"); 
	return -1;
    }

    // Initialize lock
    lock->counter = 0;
    lock->turn = 0;

    // Create threads
    int thread_ret;
    int i;
    for (i = 0; i < thread_count; i++) {
    	thread_ret = pthread_create(&threads[i], NULL, routine, NULL);
	if (thread_ret != 0) {
	    fprintf(stderr, "Error creating thread.\n");
	    return -1;
	}
    }

    // Sleep the main thread then signal the threads to finish
    int sleep_ret;
    sleep_ret = sleep(seconds);
    if (sleep_ret != 0) {
	fprintf(stderr, "Error main thread interrupted.\n");
	return -1;
    }
    run_threads = 0;

    // Wait for all the threads to complete
    ret_t *ret;
    for (i = 0; i < thread_count; i++) {
	thread_ret = pthread_join(threads[i], (void **) &ret);
	if (thread_ret != 0) {
		fprintf(stderr, "Error joining thread.\n");
		return -1;
	}
	printf("Thread %d entered the critical section %d times.\n", i, ret->cs_count);
    }

    // Free memory
    free(threads);
    free((spin_lock_t*)lock);

    return 0;
}

/* A routine that spins until run_threads is false. 
 * checks  mutual exclusion amoung all the threads that call it. */
void *routine() {
    ret_t *ret = malloc(sizeof(ret_t));
    if (ret == NULL) {
	fprintf(stderr, "Error allocating memory.\n"); 
    }
    ret->cs_count = 0;
    while (run_threads) {
	spin_lock(lock);
	assert(in_cs == 0);
	in_cs++;
	assert(in_cs == 1);
	in_cs++;
	assert(in_cs == 2);
	in_cs++;
	assert(in_cs == 3);
	in_cs = 0;
	spin_unlock(lock);
	ret->cs_count++;
    }
    return ret;
}

/*
 * atomic_xadd
 *
 * equivalent to atomic execution of this code:
 *
 * return (*ptr)++;
 *
 */
static inline int atomic_xadd (volatile int *ptr)
{
  register int val __asm__("eax") = 1;
  asm volatile ("lock xaddl %0,%1"
    : "+r" (val)
    : "m" (*ptr)
    : "memory");  
  return val;
}

void spin_lock(volatile spin_lock_t *s) {
    int me;
    int ret;
    me = atomic_xadd(&(s->counter));
    while (me != s->turn) {
	ret = sched_yield();
	if (ret != 0) {
	    fprintf(stderr, "Error yielding processor.\n");
	}
    }

}

void spin_unlock(volatile spin_lock_t *s) {
    s->turn++;
}
