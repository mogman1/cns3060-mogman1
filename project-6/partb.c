#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 4

int count;

void* incrementCounter( void* m ) {
	int i;
	for (i = 0; i < 10; ++i) {
		int tempValue = count;
		sleep(1);
		tempValue = tempValue + 1;
		count = tempValue;
	}

	return NULL;
}

int main() {
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

	return 0;
}
