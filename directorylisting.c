#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

struct path {
	char curPath[FILENAME_MAX];
	struct path *Next;
};

struct MyStack {
	struct path *head;
	int size;
};

int lstat(const char *path, struct stat *buf);
int push(struct MyStack *, char *);
char * pop(struct MyStack *, char *);
void stack_init(struct MyStack *);
void walktree(struct MyStack *stack, char *path);


int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("not enough argument\n");
		return -1;
	}

	char path[FILENAME_MAX];
	struct MyStack stack;

	stack_init(&stack);

	// add first dir on stack
	if (push(&stack, argv[1])) {
		printf("can't push");
		return -1;
	}

	while (stack.size != 0) {
		if (pop(&stack, path) == NULL) {
			printf("can't pop from empty stack");
			return -1;
		}
		printf("%s\n", path);
		walktree(&stack, path);
		path[0] = '\0';
	}
	
	return 0;
}

// print all content in dir and push dir on stack
void walktree(struct MyStack *stack, char *path) {

	DIR *dp;

	dp = opendir(path);
	if (dp == NULL) {
		return;
	}

	struct stat buf;
	struct dirent *ep;
	char curPath[FILENAME_MAX];

	while ((ep = readdir(dp)) != NULL) {
		if (strcmp(ep->d_name, ".") == 0 ||
				strcmp(ep->d_name, "..") == 0) {
			continue;
		}
		strcat(curPath, path);     
		strcat(curPath, "/");
		strcat(curPath, ep->d_name);

		//  lstat fills structure pointed to
		// by buf about named file
		if (lstat(curPath, &buf) == -1) {
			printf("can't get file info %s\n", path);
			curPath[0] = '\0';
			continue;
		}
	
		if (S_ISDIR(buf.st_mode)) {
			if (push(stack, curPath)) {
				printf("can't push %s\n", curPath);
			}
			printf("   %s\n", ep->d_name);
		}
		else {
			printf("   %s\n", ep->d_name);
		}
		curPath[0] = '\0';
	}
	closedir(dp);
	return;
}

void stack_init(struct MyStack *stack) {

	stack->head = NULL;
	stack->size = 0;
}

int push(struct MyStack *stack, char *path) {
	
	if (strlen(path) > FILENAME_MAX) {
		return -1;
	}

	struct path *p = (struct path*)malloc(sizeof(struct path));
	if (p == NULL) {
		return -1;
	}

	// add path to p.curPaht
	strcpy(p->curPath, path);
	p->Next = stack->head;
	stack->head = p;
	stack->size++;

	return 0;
}

char * pop(struct MyStack *stack, char *path) {
	
	if (stack->size == 0) {
		return NULL;
	}

	struct path *p;

	p = stack->head;
	stack->head = p->Next;
	strcpy(path, p->curPath);

	free(p);
	stack->size--;

	return path;
}
/*
Code in this post represents basic directory-listing program and show how to get information about files. It is  also implemented stack c data structure.  Executing this code will write all files in directory and subdirectory too standard output.  There is one very important lesson I was learned from writing this code. Function free() doesn’t “erase content” on specific address in memory but only make it available. Let’s look what will happen if we use strcat() function instead strcpy() in line 117.
push “aaa”
push “bbb”
push “ccc”
pop
	“ccc”
push “ddd”
pop
	“cccddd”
Function strcat() concatenate two strings, because “ccc” content is still in memory after pop, second call of pop returns “cccddd” instead “ddd”. 
*/
