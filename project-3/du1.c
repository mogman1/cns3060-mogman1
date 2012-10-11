/*
 * du1.c
 *
 *   Created on: Sep 27, 2012
 *       Author: Shaun Carlson
 *   Student ID: 10576840
 *  Description: Mimics *nix program "du"
 */

#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CURRENT_DIR "."

int displayFileStats(const char*);

int main(int argc, char* argv[]) {
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			displayFileStats(argv[i]);
		}
	} else {
		displayFileStats(CURRENT_DIR);
	}

	return 0;
}

/*
 * Displays the file size and name of the file/directory referenced by path.  If path represents
 * a directory, the function recursively calls itself to traverse all elements in the directory
 * tree.
 */
int displayFileStats(const char* path) {
	struct stat fileStats;
	stat(path, &fileStats);
	if (S_ISDIR(fileStats.st_mode)) {
		DIR* dirHandle = opendir(path);
		if (dirHandle == NULL) {
			printf("du1: %s: Permission denied\n", path);
		} else {
			struct dirent *dirEntry;
			while ((dirEntry = readdir(dirHandle)) != NULL) {
				if (strcmp("..", dirEntry->d_name) == 0) continue; //ignore entries that take you to the parent dir
				if (strcmp(".", dirEntry->d_name) == 0)  continue; //ignore self reference on directories

				//we cannot predict the length of a directory string, so it must be dynamically allocated
				int dirStringLength = strlen(path) + strlen(dirEntry->d_name) + 2; //one for separating '/' and \0 char
				char* newPath = (char*)malloc(dirStringLength);
				if (newPath == NULL) {
					printf("Failure to allocate memory for path\n");
					exit(EXIT_FAILURE);
				}

				memset(newPath, '\0', dirStringLength); //initialize string
				sprintf(newPath, "%s/%s", path, dirEntry->d_name);
				displayFileStats(newPath);
				free(newPath); //free memory
			}

			if (closedir(dirHandle) != 0) {
				printf("Error closing directory %s\n", path);
			}
		}
	}

	printf("%5lld B %s\n", fileStats.st_size, path);

	return 0;
}
