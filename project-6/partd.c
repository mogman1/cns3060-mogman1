#include <stdio.h>
#include <unistd.h>

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
	return 0;
}
