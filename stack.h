#ifndef STACK_H
#define STACK_H

#include "string.h"

typedef struct element {
	void* value;
	struct element* next;
} Element;

typedef struct stack {
	Element* head;
	Element* node;
} Stack;

int stack_empty(Stack* s);
void stack_init(Stack* s);
void stack_free(Stack* s);
void stack_push(Stack* s, void* ch);
void* stack_pop(Stack* s);
void* stack_top(Stack* s);

#endif
