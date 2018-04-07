/* author: jake pitkin
 * last edit: april 2 2018
 * assignment 4 - problem 4
 * cs5460 - operating systems
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sched.h>

pthread_mutex_t *lock;
long thread_count;
long seconds;
volatile int run_threads;

volatile unsigned long samples;
volatile unsigned long inside_count;

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
    lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    if (threads == NULL || lock == NULL) {
	fprintf(stderr, "Error allocating memory.\n"); 
	return -1;
    }

    samples = 0;
    inside_count = 0;
    srand(time(NULL));


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
    for (i = 0; i < thread_count; i++) {
	thread_ret = pthread_join(threads[i], NULL);
	if (thread_ret != 0) {
		fprintf(stderr, "Error joining thread.\n");
		return -1;
	}
    }

    float pi = (4 * inside_count)/(float)samples;
    printf("The estimated value of pi is: %f\n", pi);

    // Free memory
    free(threads);
    free((pthread_mutex_t*)lock);

    return 0;
}

/* Generates random points inside a 2x2 square.
 * Add to 'samples' the number of samples generated and
 * adds to 'inside_count' the number of points that land
 * inside a unit circle inside the square. Locks are
 * used to protect the global counts. */
void *routine() {
    float x;
    float y;
    while (run_threads) {
	// Random (x, y) point in the range [-1, 1]
	// With help from stackoverflow
	x = ((float)rand()/(float)(RAND_MAX)) * 2 - 1;
	y = ((float)rand()/(float)(RAND_MAX)) * 2 - 1;
	pthread_mutex_lock(lock);
	if ((x*x + y*y) < 1)
	    inside_count++;
	samples++;
	pthread_mutex_unlock(lock);
    }
    return NULL;
}
