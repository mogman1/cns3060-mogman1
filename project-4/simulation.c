/*
 * simulation.c
 *
 *   Created on: Oct 23, 2012
 *       Author: Shaun Carlson
 *   Student ID: 10576840
 *  Description: Simulates different disk scheduling algorithms
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USER_INPUT 100
#define ARM_START_POSITION 15

void firstComeFirstServed(int*, int);
void shortestSeekTime(int*, int);
void elevator(int*, int);
int compare(const void*, const void*);

int main() {
	int input[MAX_USER_INPUT];
	char inputString[100];
	memset(inputString, '\0', 100);
	int inputReceived = 0;

	while (fgets(inputString, 100, stdin) != NULL) {
		int temp = atoi(inputString);
		if (temp < 0) break;
		if ((inputReceived + 1) == MAX_USER_INPUT) {
			printf("Cannot receive more than 100 numbers.  Please input a negative number to begin simulation.\n");
		}

		input[inputReceived] = temp;
		inputReceived++;
	}

	firstComeFirstServed(input, inputReceived);
	    shortestSeekTime(input, inputReceived);
	            elevator(input, inputReceived);
	return 0;
}

/*
 * Simulates the first-come-first-served algorithm for disk scheduling.
 */
void firstComeFirstServed(int* array, int arraySize) {
	int totalTracksTraversed = 0;
	printf("First Come First Served\n");
	printf("Head Movement\t\tTracks Traversed\n");
	if (arraySize == 0) {
		printf("Tracks %3d - %-3d\t0\n", ARM_START_POSITION, ARM_START_POSITION);
	} else {
		int arrayPos = 0;
		int currentPosition = ARM_START_POSITION;
		for (arrayPos = 0; arrayPos < arraySize; arrayPos++) {
			int tracksTraversed = abs((currentPosition - array[arrayPos]));
			printf("Tracks %3d - %-3d\t%d\n", currentPosition, array[arrayPos], tracksTraversed);
			currentPosition = array[arrayPos];
			totalTracksTraversed += tracksTraversed;
		}
	}

	printf("\n");
	printf("Total Tracks Traversed\t%d\n\n", totalTracksTraversed);
}

/*
 * Simulates the shortest seek time algorithm for disk scheduling.
 */
void shortestSeekTime(int* array, int arraySize) {
	int totalTracksTraversed = 0;

	printf("Shortest Seek Time");
	printf("Head Movement\t\tTracks Traversed\n");
	if (arraySize == 0) {
		printf("Tracks %3d - %-3d\t0\n", ARM_START_POSITION, ARM_START_POSITION);
	} else {
		/*
		 * Create an array of flags to keep track of what has been done already.
		 * The array is the same size as the maximum input, which may be larger
		 * than the array, but arraySize keeps us from going out of bounds.
		 */
		int t;
		int valuesChecked[MAX_USER_INPUT];
		for (t = 0; t < MAX_USER_INPUT; t++) valuesChecked[t] = 0;

		//keep iterating through the array until all tracks have been processed
		int currentPosition = ARM_START_POSITION;
		int tracksLeft;
		for (tracksLeft = arraySize; tracksLeft > 0; tracksLeft--) {
			//go through the array to determine what the shortest seek time is and record that index
			int shortestSeekTimeIndex = 0;
			int shortestSeekTime = INT_MAX;
			int temp = 0;
			for (t = 0; t < arraySize; t++) {
				if (valuesChecked[t]) continue; //ignore already done values
				temp = abs((currentPosition - array[t]));
				if (temp < shortestSeekTime) {
					shortestSeekTime = temp;
					shortestSeekTimeIndex = t;
				}
			}

			printf("Tracks %3d - %-3d\t%d\n", currentPosition, array[shortestSeekTimeIndex], shortestSeekTime);
			currentPosition = array[shortestSeekTimeIndex];
			totalTracksTraversed += shortestSeekTime;
			valuesChecked[shortestSeekTimeIndex] = 1; //flag this index as done
		}
	}

	printf("\n");
	printf("Total Tracks Traversed\t%d\n\n", totalTracksTraversed);
}

/*
 * Simulates the elevator algorithm for disk scheduling going up first and then down.
 * NOTE: This method sorts the passed in array, so make sure you only pass in a copy
 * of the array if you need it for later values.
 */
void elevator(int* array, int arraySize) {
	int totalTracksTraversed = 0;
	printf("Elevator");
	printf("Head Movement\t\tTracks Traversed\n");

	if (arraySize == 0) {
		printf("Tracks %3d - %-3d\t0\n", ARM_START_POSITION, ARM_START_POSITION);
	} else {
		//Sort for simplicity of the algorithm
		qsort (array, arraySize, sizeof(int), compare);

		/*
		 * Determine where in the array the starting position 15 would fall.
		 * Initiate to -1 because the starting position could be less all
		 * values in the array
		 */
		int midIndex = -1;
		int t;
		for (t = 0; t < arraySize; t++) {
			if (array[t] < ARM_START_POSITION) midIndex = t;
		}

		//the elevator goes up from the starting position
		int currentPosition = ARM_START_POSITION;
		for (t = midIndex+1; t < arraySize; t++) {
			int diff = abs(currentPosition - array[t]);
			printf("Tracks %3d - %-3d\t%d\n", currentPosition, array[t], diff);
			currentPosition = array[t];
			totalTracksTraversed += diff;
		}

		//and then the elevator goes down from whatever the max value was
		for (t = midIndex; t >= 0; t--) {
			int diff = abs(currentPosition - array[t]);
			printf("Tracks %3d - %-3d\t%d\n", currentPosition, array[t], diff);
			currentPosition = array[t];
			totalTracksTraversed += diff;
		}
	}

	printf("\n");
	printf("Total Tracks Traversed\t%d\n\n", totalTracksTraversed);
}

/*
 * Borrowed from qsort example at http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/
 */
int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}
