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

int StackEmpty(Stack *s);
void StackInit(Stack *s);
void StackFree(Stack *s);
void StackPush(Stack *s, void *ch);
Element* StackPop(Stack *s);
Element* StackTop(Stack *s);

#endif
