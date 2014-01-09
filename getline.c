#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 100

int getl(char **);

int main() {

	char *charbufer;
	int n;

	n =  getl(&charbufer);
	if (n > 0) {
		printf("%d\n" , n);
		puts(charbufer);
	}

	return(0);
}

// read line from stdin
int getl(char **charbufer) {
	
	char c;
	char *ptr;
	int n = 0;
	int curSize = MAXLINE;
	
	ptr = malloc(MAXLINE*sizeof(char));
	if (ptr == NULL) {
		return n;
	}

	while ((c = getchar()) != EOF && c != '\n') {
		if (n == curSize-2) {
			char *ptr2 = realloc(ptr, curSize + MAXLINE);
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

	if (c == '\n') {
		ptr[n++] = '\n';
	}
	ptr[n] = '\0';
	*charbufer = ptr;
	
	return n;
}


