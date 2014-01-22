#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 100

// read line from stdin
int getL(char **charbufer) {
	
	char c;
	char *ptr, *ptr2;
	int n = 0;
	int curSize = MAXLINE;
	
	ptr = malloc(MAXLINE*sizeof(char));
	if (ptr == NULL) {
		return n;
	}

	while ((c = getchar()) != EOF && c != '\n') {
		if (n == curSize-2) {
			ptr2 = realloc(ptr, curSize + MAXLINE);
			if (ptr2 == NULL) {
				ptr[n] = '\0';
				*charbufer = ptr; 
				return n;
			}
			else {
				ptr = ptr2;
				curSize += MAXLINE;
			}
		}
		ptr[n++] = c;
	}

	ptr[n] = '\0';
	*charbufer = ptr;

	if (c == EOF) {
		n = -1;
	}
	
	return n;
}

int main() {

	char *secret = getenv("HOME");
	
	if (secret == NULL) {
		puts("no bash variable");
		return 0;
	}

	char *charbufer;
	char *find = NULL;
	int n;

	while (find == NULL) {
		n = getL(&charbufer);
		if (n == -1) {
			free(charbufer);
			break;
		}
		find = strstr(charbufer, secret);
		free(charbufer);
	}

	return 0;
}
