/* author: jake pitkin
 * last edit: april 25 2018
 * assignment 5 - problem 1
 * cs5460 - operating systems
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

typedef struct __ret_t {
    unsigned long n_play;
} ret_t;

volatile int n_cats;
volatile int n_dogs;
volatile int n_birds;

volatile int cats = 0;
volatile int dogs = 0;
volatile int birds = 0;

volatile int run_threads = 1;

pthread_cond_t cv_cat = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv_dog = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv_bird = PTHREAD_COND_INITIALIZER;
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

    int i;
    int ret;
    // Create threads
    for (i = 0; i < n_cats; i++) {
    	ret = pthread_create(&cat_threads[i], NULL, cat_routine, NULL);
	if (ret != 0) {
	    fprintf(stderr, "Error creating thread.\n");
	    return -1;
	}
    }

    for (i = 0; i < n_dogs; i++) {
    	ret = pthread_create(&dog_threads[i], NULL, dog_routine, NULL);
	if (ret != 0) {
	    fprintf(stderr, "Error creating thread.\n");
	    return -1;
	}
    }

    for (i = 0; i < n_birds; i++) {
    	ret = pthread_create(&bird_threads[i], NULL, bird_routine, NULL);
	if (ret != 0) {
	    fprintf(stderr, "Error creating thread.\n");
	    return -1;
	}
    }

    // Sleep the main thread for 10 seconds
    ret = sleep(10);
    if (ret != 0) {
	fprintf(stderr, "Error main thread interrupted.\n");
	return -1;
    }
    run_threads = 0;

    // Wait for all the threads to complete
    unsigned long n_cat_play = 0;
    ret_t *cat_ret;
    for (i = 0; i < n_cats; i++) {
	ret = pthread_join(cat_threads[i], (void **) &cat_ret);
	if (ret != 0) {
	    fprintf(stderr, "Error joining thread.\n");
	    return -1;
	}
	n_cat_play += cat_ret->n_play;
    }

    unsigned long n_dog_play = 0;
    ret_t *dog_ret;
    for (i = 0; i < n_dogs; i++) {
	ret = pthread_join(dog_threads[i], (void **) &dog_ret);
	if (ret != 0) {
	    fprintf(stderr, "Error joining thread.\n");
	    return -1;
	}
	n_dog_play += dog_ret->n_play;
    }

    unsigned long n_bird_play = 0;
    ret_t *bird_ret;
    for (i = 0; i < n_birds; i++) {
	ret = pthread_join(bird_threads[i], (void **) &bird_ret);
	if (ret != 0) {
	    fprintf(stderr, "Error joining thread.\n");
	    return -1;
	}
	n_bird_play += bird_ret->n_play;
    }
    printf("cat play = %lu, dog play = %lu, bird play = %lu\n", n_cat_play,
	    n_dog_play, n_bird_play);

    // Free memory
    free(cat_threads);
    free(dog_threads);
    free(bird_threads);
    free(cat_ret);
    free(dog_ret);
    free(bird_ret);

    return 0;
}

void *cat_routine(void *args) {
    ret_t *ret = malloc(sizeof(ret_t));
    if (ret == NULL) {
	fprintf(stderr, "Error allocating memory.\n"); 
    }
    ret->n_play = 0;
    while (run_threads) {
	cat_enter();
	play();
	ret->n_play++;
	cat_exit();
    }
    return ret;
}

void *dog_routine(void *args) {
    ret_t *ret = malloc(sizeof(ret_t));
    if (ret == NULL) {
	fprintf(stderr, "Error allocating memory.\n"); 
    }
    ret->n_play = 0;
    while (run_threads) {
	dog_enter();
	play();
	ret->n_play++;
	dog_exit();
    }
    return ret;
}

void *bird_routine(void *args) {
    ret_t *ret = malloc(sizeof(ret_t));
    if (ret == NULL) {
	fprintf(stderr, "Error allocating memory.\n"); 
    }
    ret->n_play = 0;
    while (run_threads) {
	bird_enter();
	play();
	ret->n_play++;
	bird_exit();
    }
    return ret;
}

void cat_enter(void) {
    pthread_mutex_lock(&mutex);
    while (dogs > 0 || birds > 0)
	pthread_cond_wait(&cv_cat, &mutex);
    cats++;
    pthread_mutex_unlock(&mutex);
}

void dog_enter(void) {
    pthread_mutex_lock(&mutex);
    while (cats > 0)
	pthread_cond_wait(&cv_dog, &mutex);
    dogs++;
    pthread_mutex_unlock(&mutex);
}

void bird_enter(void) {
    pthread_mutex_lock(&mutex);
    while (cats > 0)
	pthread_cond_wait(&cv_bird, &mutex);
    birds++;
    pthread_mutex_unlock(&mutex);
}

void cat_exit(void) {
    pthread_mutex_lock(&mutex);
    cats--;
    pthread_cond_broadcast(&cv_dog);
    pthread_cond_broadcast(&cv_bird);
    pthread_mutex_unlock(&mutex);
}

void dog_exit(void) {
    pthread_mutex_lock(&mutex);
    dogs--;
    pthread_cond_broadcast(&cv_cat);
    pthread_mutex_unlock(&mutex);
}

void bird_exit(void) {
    pthread_mutex_lock(&mutex);
    birds--;
    pthread_cond_broadcast(&cv_cat);
    pthread_mutex_unlock(&mutex);
}

void play(void) {
    for (int i=0; i<10; i++) {
	assert(cats >= 0 && cats <= n_cats);
	assert(dogs >= 0 && dogs <= n_dogs);
	assert(birds >= 0 && birds <= n_birds);
	assert(cats == 0 || dogs == 0);
	assert(cats == 0 || birds == 0);
    }
}
