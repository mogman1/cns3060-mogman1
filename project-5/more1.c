/*
 * more.c
 *
 *   Created on: Oct 30, 2012
 *       Author: Shaun Carlson
 *   Student ID: 10576840
 *  Description: Simulates the *nix "more" program
 */

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define LINES_PER_SCREEN 23
#define STDIN_ATTRS 0

/*
 * Holds the settings for the terminal prior to altering them for use by more1
 */
struct termios savedAttrs;

/*
 * The file name, if specified.  This is global so that processMore can reference it
 * when displaying the first prompt
 */
char* fileName;

/*
 * File descriptor for the terminal.  This is global so that the toggle and restore
 * terminal functions can properly save/load attributes
 */
int   fd_tty;

/*
 * The opened file descriptor.  This is global so that processMore can grab characters
 * from the terminal rather than stdin.
 */
FILE* fp_tty;

int  clearPrompt(const char*);
int  processMore(FILE*);
void restoreTerminalAttrs(int);
int  restoreTerminalAttrs();
int  toggleTerminalAttrs();

/*
 * Determines whether to run more1 from a file or from stdin
 */
int main(int argc, char* argv[]) {
	//store terminal attributes and set up signal intercepts
	fd_tty = open("/dev/tty", O_RDWR);
	fp_tty = fdopen(fd_tty, "r+");
	if (toggleTerminalAttrs() != 0) return 1;
	signal(SIGINT,  restoreTerminalAttrs);
	signal(SIGTERM, restoreTerminalAttrs);

	//determine if we're opening a file or using stdin
	FILE* fp;
	if (argc > 1) {
		fileName = argv[1];
		if ((fp = fopen(fileName, "r")) == NULL) {
			perror("Unable to open file\n");
			return 1;
		}
	} else {
		fp = stdin;
	}

	processMore(fp);
	fclose(fp);

	return restoreTerminalAttrs();
}

/*
 * Clears out the last line, which happens to be the prompt, so that the next line
 * can be displayed by more1.  THe actual prompt should be passed in so that
 * this function can clear out the correct number of characters, or a pseudo-prompt
 * large enough to handle the job.
 */
int clearPrompt(const char* prompt) {
	int i = 0;
	putc('\r', stdout);
	for (i = 0; prompt[i] != '\0'; i++) putc(' ', stdout);
	putc('\r', stdout);

	return 0;
}

/*
 * Does the actual processing of the more1 program.  This displays lines, either from
 * stdin or a file which was opened by main and passed as src.  If there are more lines
 * still to display, a prompt giving some information about what was read is displayed
 * and the program awaits for user input.
 */
int processMore(FILE* src) {
	char line[1000];
	int currentLine = 0;
	int bytes = 0;
	int totalLines = 0;

	//get total lines in file, if it is a file
	if (src != stdin) {
		while (fgets(line, 1000, src) != NULL) totalLines++;
		rewind(src);
	}

	//linesToPrint determines how many lines to print before getting user input again
	int linesToPrint = LINES_PER_SCREEN;
	while (fgets(line, 1000, src) != NULL) {
		printf("%s", line);
		currentLine++;
		linesToPrint--;
		if (src == stdin) {
			//after fetching the line, determine how large it is in bytes
			int t = 0;
			int i;
			for (i = 0; line[i] != '\0' && i < 1000; i++) t++;
			bytes += sizeof(char) * t;
		}

		if (!linesToPrint) {
			char prompt[200];
			if (src == stdin) {
				//prompt to show when we're reading from stdin
				snprintf(prompt, 200, "bytes %i", bytes);
			} else if (currentLine == LINES_PER_SCREEN) {
				//initial prompt when dealing with a file
				float percentOfFile = ((float)currentLine / (float)totalLines) * 100;
				snprintf(prompt, 200, "%s %.0f%%", fileName, percentOfFile);
			} else {
				//prompt to show after the initial prompt
				float percentOfFile = ((float)currentLine / (float)totalLines) * 100;
				snprintf(prompt, 200, "%.0f%%", percentOfFile);
			}

			//display prompt in reverse video
			printf("\033[7m%s\033[m", prompt);
			int getInput = 1;
			while (getInput) {
				//read user input
				char cmd = getc(fp_tty);
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

/*
 * Restores the original terminal settings that were saved at the start
 * of the program
 */
int restoreTerminalAttrs() {
	if (tcsetattr(fd_tty, TCSANOW, &savedAttrs) == -1) {
		perror("Error restoring terminal attributes\n");
		return 1;
	}

	return 0;
}

/*
 * Function used when intercepting SIGINT and SIGTERM.  It basically
 * just wraps the other restoreTerminalAttrs function, while issuing a command
 * to clear the prompt.
 */
void restoreTerminalAttrs(int) {
	clearPrompt("                            "); //clear prompt line
	restoreTerminalAttrs();
	exit(EXIT_SUCCESS);
}

/*
 * Saves the attributes of the terminal for restoration later.  Then modifies
 * terminal settings as needed by more1
 */
int toggleTerminalAttrs() {
	if (tcgetattr(fd_tty, &savedAttrs) == -1) {
		perror("Error saving terminal attributes\n");
		return 1;
	}

	struct termios newSettings;
	if (tcgetattr(fd_tty, &newSettings) == -1) {
		perror("Error updating terminal attributes\n");
		return 1;
	}

	newSettings.c_lflag &= ~ECHO;   //turn off echo
	newSettings.c_lflag &= ~ICANON; //turn off canonical mode
	if (tcsetattr(fd_tty, TCSANOW, &newSettings) == -1) {
		perror("Error updating terminal attributes\n");
		return 1;
	}

	return 0;
}
