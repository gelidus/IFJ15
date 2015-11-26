#ifndef STACK_H
#define STACK_H

#include "string.h"

typedef struct element {
	void* value;
	struct element* next;
} Element;

typedef struct stack {
	Element* head;
	int size;
} Stack;

int StackEmpty(Stack *s);
void StackInit(Stack *s);
void StackFree(Stack *s);
void StackPush(Stack *s, void *ch);
void* StackPop(Stack *s);
void* StackTop(Stack *s);
int StackSize(Stack *s);
Element* StackTopElement(Stack *s);

#endif
