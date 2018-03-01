#ifndef COMMON_HEADERS
	#define COMMON_HEADERS
	#include <stdlib.h>
	#include <string.h>
	#define maxContentLen 255
#endif

typedef struct stakrec {
	char content[maxContentLen];
	char type[maxContentLen];
	struct stakrec *prev;
} mystack;
typedef mystack *stack;

void copyobj (char* element1, char* element2) { strcpy (element1, element2); }
void initialize (stack *s) { (*s) = NULL; }
int stackempty (stack s) { return s == NULL; }

void push (stack *s, char* type, char* content) {
	mystack *a;
	a = (mystack *) malloc(sizeof(mystack));
	a->prev = *s;
	copyobj (a->content, content);
	copyobj (a->type, type);
	*s = a;
}

int pop (stack *s){
	mystack *ptr;
	if(stackempty(*s)) return 0;
	else {
		ptr = *s;
		(*s) = (*s)->prev;
		free(ptr);
		return 1;
	}
}

int top(stack s, char* content, char* type) {
	if (stackempty(s)) return 0;
	else {
		copyobj (content, s->content);
		copyobj (type, s->type);
		return 1;
	}
}
