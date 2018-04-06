/* author: jake pitkin
 * last edit: april 2 2018
 * assignment 4 - problem 2
 * cs5460 - operating systems
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sched.h>

typedef struct __args_t {
    int tid;
} args_t;

typedef struct __ret_t {
    int cs_count;
} ret_t;

volatile int *entering;
volatile int *number;
volatile int in_cs;
long thread_count;
long seconds;
int run_threads;

void lock(int i);
void unlock(int i);
void *routine(void *arg);
int max(int arr[], size_t size);

int main(int argc, char *argv[]) {
    // Validate input
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments.\n");
        return -1;
    }
    thread_count = strtol(argv[1], NULL, 10);
    seconds = strtol(argv[2], NULL, 10);
    run_threads = 1;
    if (thread_count == 0) {
        fprintf(stderr, "Invalid input for threads. Expected [1, 99].\n");
	return -1;
    }
    if (seconds == 0) {
        fprintf(stderr, "Invalid input for seconds. Expected positive integer.\n");
        return -1;
    }

    // Allocate global variables
    entering = (int*) malloc(thread_count*sizeof(int));
    number = (int*) malloc(thread_count*sizeof(int));
    if (entering == NULL || number == NULL) {
	fprintf(stderr, "Error allocating memory.\n"); 
	return -1;
    }

    // Allocate threads
    args_t *tids = (args_t*) malloc(thread_count*sizeof(args_t));
    pthread_t *threads = (pthread_t*) malloc(thread_count*sizeof(pthread_t));
    if (tids == NULL || threads == NULL) {
	fprintf(stderr, "Error allocating memory.\n"); 
	return -1;
    }


    // Initialize
    int i;
    for (i = 0; i < thread_count; i++) {
	entering[0] = 0;
	number[0] = 0;
	tids[i].tid = i;
    }

    // Create threads
    int thread_ret;
    for (i = 0; i < thread_count; i++) {
    	thread_ret = pthread_create(&threads[i], NULL, routine, (void*) &tids[i]);
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
	fprintf(stderr, "Thread %d entered the critical section %d times.\n", i, ret->cs_count);
    }

    // Free memory
    free((int*) entering);
    free((int*) number);
    free(tids);
    free(threads);

    return 0;
}

/* Locking mechanism for Lamport's bakery algorithm. */
void lock(int i) {
    entering[i] = 1;
    number[i] = 1 + max((int*)number, thread_count);
    entering[i] = 0;
    int j;
    for (j = 0; j < thread_count; j++) {
	// Wait until thread j receives its number
	while (entering[j]) { /* Waiting */ }
	// Wait until all threads with smaller numbers or with the same
	// number, but with high priority, finish their work
	while ((number[j] != 0) && ((number[j] < number[i]) || ((number[j] == number[i]) && (j < i)))) { /* Waiting */ }
    }
    return;
}

/* Unlocking mechanism for Lamport's bakery algorithm. */
void unlock(int i) {
    number[i] = 0;
}

/* A routine that spins until run_threads is false. 
 * checks  mutual exclusion amoung all the threads that call it. */
void *routine(void *arg) {
    args_t *a = (args_t*) arg;
    int i = a->tid;
    ret_t *ret = malloc(sizeof(ret_t));
    if (ret == NULL) {
	fprintf(stderr, "Error allocating memory.\n"); 
    }
    ret->cs_count = 0;
    while (run_threads) {
	lock(i);
	ret->cs_count++;
	assert(in_cs == 0);
	in_cs++;
	assert(in_cs == 1);
	in_cs++;
	assert(in_cs == 2);
	in_cs++;
	assert(in_cs == 3);
	in_cs = 0;
	unlock(i);
    }
    return ret;
}

/* Returns the largest integer in an array.
   If the array is empty then -EINVAL is returned. */
int max(int arr[], size_t size) {
    if (size == 0)
	return -1;
    int max = arr[0];
    unsigned int i;
    for (i = 1; i < size; i++) {
	if (arr[i] > max)
	    max = arr[i];
    }
    return max;
}
