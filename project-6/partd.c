#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 4

int count;
pthread_mutex_t incrementLock;

void* incrementCounter( void* m ) {
	int i;
	for (i = 0; i < 10; ++i) {
		pthread_mutex_lock(&incrementLock);
		int tempValue = count;
		tempValue = tempValue + 1;
		count = tempValue;
		pthread_mutex_unlock(&incrementLock);
		sleep(1);
	}

	return NULL;
}

int main() {
	pthread_mutex_init(&incrementLock, NULL);
	pthread_t threads[NUM_THREADS];
	int i, thread_code;
	for (i = 0; i < NUM_THREADS; i++) {
		thread_code = pthread_create(&threads[i], NULL, incrementCounter, (void *)i);
		if (thread_code) {
			char err[100];
			sprintf(err, "ERROR: Unable to start thread [code: %d]\n", thread_code);
			perror(err);
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < NUM_THREADS; i++) {
		thread_code = pthread_join(threads[i], NULL);
		if (thread_code) {
			char err[100];
			sprintf(err, "ERROR: Unable to join thread [code: %d]\n", thread_code);
			perror(err);
			exit(EXIT_FAILURE);
		}
	}

	printf("Count: %d\n", count);
	pthread_mutex_destroy(&incrementLock);

	return 0;
}
