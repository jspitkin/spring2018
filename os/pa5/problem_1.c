/* author: jake pitkin
 * last edit: april 2 2018
 * assignment 5 - problem 1
 * cs5460 - operating systems
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

typedef struct __ret_t {
    int cs_count;
} ret_t;

volatile int n_cats;
volatile int n_dogs;
volatile int n_birds;

volatile int cats = 0;
volatile int dogs = 0;
volatile int birds = 0;

volatile int cat_play_cnt = 0;
volatile int dog_play_cnt = 0;
volatile int bird_play_cnt = 0;

volatile int run_threads = 1;

pthread_cond_t cat = PTHREAD_COND_INITIALIZER;
pthread_cond_t dog = PTHREAD_COND_INITIALIZER;
pthread_cond_t bird = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *cat_routine(void *args);
void *dog_routine(void *args);
void *bird_routine(void *args);

void cat_enter(void);
void dog_enter(void);
void bird_enter(void);

void cat_exit(void);
void dog_exit(void);
void bird_exit(void);

void play(void);

int main(int argc, char *argv[]) {
    // Validate input
    if (argc != 4) {
        fprintf(stderr, "Invalid number of arguments.\n");
        return -1;
    }
    n_cats = strtol(argv[1], NULL, 10);
    n_dogs = strtol(argv[2], NULL, 10);
    n_birds = strtol(argv[3], NULL, 10);
    if (n_cats < 0 || n_cats > 99) {
        fprintf(stderr, "Invalid number of cats. Expected [0, 99].\n");
	return -1;
    }
    if (n_dogs < 0 || n_dogs > 99) {
        fprintf(stderr, "Invalid number of dogs. Expected [0, 99].\n");
	return -1;
    }
    if (n_birds < 0 || n_birds > 99) {
        fprintf(stderr, "Invalid number of birds. Expected [0, 99].\n");
	return -1;
    }

    // Allocate threads
    pthread_t *cat_threads = (pthread_t*) malloc(n_cats*sizeof(pthread_t));
    pthread_t *dog_threads = (pthread_t*) malloc(n_dogs*sizeof(pthread_t));
    pthread_t *bird_threads = (pthread_t*) malloc(n_birds*sizeof(pthread_t));
    if (cat_threads == NULL || dog_threads == NULL || bird_threads == NULL) {
	fprintf(stderr, "Error allocating memory.\n"); 
	return -1;
    }

    // Create threads
    int i;
    int thread_ret;
    for (i = 0; i < n_cats; i++) {
    	thread_ret = pthread_create(&threads[i], NULL, play, NULL);
	if (thread_ret != 0) {
	    fprintf(stderr, "Error creating thread.\n");
	    return -1;
	}
    }
    for (i = 0; i < n_dogs; i++) {
    	thread_ret = pthread_create(&threads[i], NULL, play, NULL);
	if (thread_ret != 0) {
	    fprintf(stderr, "Error creating thread.\n");
	    return -1;
	}
    }
    for (i = 0; i < n_birds; i++) {
    	thread_ret = pthread_create(&threads[i], NULL, play, NULL);
	if (thread_ret != 0) {
	    fprintf(stderr, "Error creating thread.\n");
	    return -1;
	}
    }

    // Sleep the main thread
    int sleep_ret;
    sleep_ret = sleep(10);
    if (sleep_ret != 0) {
	fprintf(stderr, "Error main thread interrupted.\n");
	return -1;
    }

    // Wait for all the threads to complete
    for (i = 0; i < animal_count; i++) {
	thread_ret = pthread_join(threads[i], NULL);
	if (thread_ret != 0) {
		fprintf(stderr, "Error joining thread.\n");
		return -1;
	}
    }

    printf("cat play = %d, dog play = %d, bird play = %d\n", cat_play_cnt,
	    dog_play_cnt, bird_play_cnt);

    // Free memory
    free(threads);

    return 0;
}

void *cat_routine(void *args) {
    return NULL;
}

void *dog_routine(void *args) {
    return NULL;
}

void *bird_routine(void *args) {
    return NULL;
}

void cat_enter(void args) {
    return NULL;
}

void dog_enter(void args) {
    return NULL;
}

void bird_enter(void args) {
    return NULL;
}

void cat_exit(void args) {
    return NULL;
}

void dog_exit(void args) {
    return NULL;
}

void bird_exit(void args) {
    return NULL;
}

void play(void) {
    for (int i=0; i<10; i++) {
	assert(cats >= 0 && cats <= n_cats);
	assert(dogs >= 0 && dogs <= n_dogs);
	assert(birds >= 0 && birds <= n_birds);
	assert(cats == 0 || dogs == 0);
	assert(cats == 0 || birds == 0);
    }
    return NULL;
}
