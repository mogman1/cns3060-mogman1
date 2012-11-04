/*
 * simulation.c
 *
 *   Created on: Oct 30, 2012
 *       Author: Shaun Carlson
 *   Student ID: 10576840
 *  Description: Simulates the *nix "more" program
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define LINES_PER_SCREEN 23
#define STDIN_ATTRS 0

struct termios savedAttrs;
char* fileName;
int clearPrompt(const char*);
int processMore(FILE*);
void restoreTerminalAttrs(int);
int restoreTerminalAttrs();
int toggleTerminalAttrs();

int main(int argc, char* argv[]) {
	//store terminal attributes and set up signal intercepts
	if (toggleTerminalAttrs() != 0) return 1;
	signal(SIGINT,  restoreTerminalAttrs);
	signal(SIGTERM, restoreTerminalAttrs);

	FILE* fp;
	if (argc > 1) {
		fileName = argv[1];
		if ((fp = fopen(fileName, "r")) == NULL) {
			perror("Unable to open file\n");
			return 1;
		}

		processMore(fp);
		fclose(fp);
	}

	printf("\n"); //ensure a new line exists for the command prompt
	return restoreTerminalAttrs();
}

int clearPrompt(const char* prompt) {
	int i = 0;
	putc('\r', stdout);
	for (i = 0; prompt[i] != '\0'; i++) putc(' ', stdout);
	putc('\r', stdout);

	return 0;
}

int processMore(FILE* src) {
	char line[1000];
	int currentLine = 0;

	//get total lines in file
	int totalLines = 0;
	while (fgets(line, 1000, src) != NULL) totalLines++;
	rewind(src);

	int linesToPrint = LINES_PER_SCREEN;
	while (fgets(line, 1000, src) != NULL) {
		printf("%s", line);
		currentLine++;
		linesToPrint--;

		if (!linesToPrint) {
			float percentOfFile = ((float)currentLine / (float)totalLines) * 100;
			char prompt[200];
			if (currentLine == LINES_PER_SCREEN) {
				snprintf(prompt, 200, "\033[7m%s %.0f%%\033[m", fileName, percentOfFile);
			} else {
				snprintf(prompt, 200, "\033[7m%.0f%%\033[m", percentOfFile);
			}

			printf("%s", prompt);
			int getInput = 1;
			while (getInput) {
				char cmd = getc(stdin);
				switch (cmd) {
					case 'q':
					case 'Q':
						clearPrompt(prompt);
						return 0;
						break;
					case '\n':
					case 13: //carriage return
						clearPrompt(prompt);
						linesToPrint = 1;
						getInput = 0;
						break;
					case 32: //space
						clearPrompt(prompt);
						linesToPrint = LINES_PER_SCREEN;
						getInput = 0;
						break;
					default:
						//all other characters ignored
						break;
				}
			}
		}
	}

	return 0;
}

int restoreTerminalAttrs() {
	if (tcsetattr(STDIN_ATTRS, TCSANOW, &savedAttrs) == -1) {
		perror("Error restoring terminal attributes\n");
		return 1;
	}

	return 0;
}

void restoreTerminalAttrs(int) {
	clearPrompt("                            "); //clear prompt line
	restoreTerminalAttrs();
	exit(EXIT_SUCCESS);
}

int toggleTerminalAttrs() {
	if (tcgetattr(STDIN_ATTRS, &savedAttrs) == -1) {
		perror("Error saving terminal attributes\n");
		return 1;
	}

	struct termios newSettings;
	if (tcgetattr(STDIN_ATTRS, &newSettings) == -1) {
		perror("Error updating terminal attributes\n");
		return 1;
	}

	newSettings.c_lflag &= ~ECHO;   //turn off echo
	newSettings.c_lflag &= ~ICANON; //turn off canonical mode
	if (tcsetattr(STDIN_ATTRS, TCSANOW, &newSettings) == -1) {
		perror("Error updating terminal attributes\n");
		return 1;
	}

	return 0;
}
