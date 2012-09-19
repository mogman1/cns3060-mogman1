/*
 * cat1.c
 *
 *   Created on: Sep 18, 2012
 *       Author: Shaun Carlson
 *   Student ID: 10576840
 *  Description: Mimics *nix program "cat"
 */

#include <stdio.h>
#include <string.h>

int outputToStdout(FILE*, int);
int main(int argc, char* argv[]) {
	int printLineNumbers = 0;
	if (argc == 1) outputToStdout(stdin, printLineNumbers); //when no params passed in
	int argCounter = 1;
	if (argc > 1) {
		if (strcmp(argv[1], "-n") == 0) {
			//consume param and set line number flag
			printLineNumbers = 1;
			argCounter++;
			if (argc == 2) outputToStdout(stdin, printLineNumbers); //when only -n passed in
		} else if (strncmp(argv[1], "-", 1) == 0 && strlen(argv[1]) > 1) {
			//checks if an invalid option was passed in
			perror("usage: ./cat1 [-n] [file ...]");
			return 1;
		}
	}

	while (argCounter < argc) {
		FILE* file;
		if (strcmp(argv[argCounter], "-") == 0) {
			file = stdin;
		} else {
			file = fopen(argv[argCounter], "r");
			if (file == NULL) {
				perror("No such file found");
				continue;
			}
		}

		outputToStdout(file, printLineNumbers);
		fclose(file);
		argCounter++;
	}

	return 0;
}

int outputToStdout(FILE* file, int printLineNumbers) {
	char currentChar;
	char prevChar = '\n';
	int lineCounter = 1;
	while ((currentChar = getc(file)) != EOF) {
		if (printLineNumbers && prevChar == '\n') printf("%6d\t", lineCounter++);
		putchar(currentChar);
		prevChar = currentChar;
	}

	if (ferror(file)) {
		perror("There was an error reading from file");
		return 1;
	}

	return 0;
}
