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

void shortestSeekTime(int* array, int arraySize) {
	int valuesChecked[MAX_USER_INPUT];
	int t;
	for (t = 0; t < MAX_USER_INPUT; t++) valuesChecked[t] = 0;
	int totalTracksTraversed = 0;

	printf("Shortest Seek Time");
	printf("Head Movement\t\tTracks Traversed\n");
	if (arraySize == 0) {
		printf("Tracks %3d - %-3d\t0\n", ARM_START_POSITION, ARM_START_POSITION);
	} else {
		int currentPosition = ARM_START_POSITION;
		int tracksLeft = arraySize;
		while (tracksLeft) {
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
			valuesChecked[shortestSeekTimeIndex] = 1;
			tracksLeft--;
		}
	}

	printf("\n");
	printf("Total Tracks Traversed\t%d\n\n", totalTracksTraversed);

}

void elevator(int* array, int arraySize) {

}
