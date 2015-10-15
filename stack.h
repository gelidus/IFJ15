#ifndef STACK_H
#define STACK_H

#include "string.h"

struct element{
	string* value;
	struct element* next;
};

struct stack{
	struct element* head;
	struct element* node;
};

int stack_empty(struct stack* s);
void stack_init(struct stack* s);
void stack_free(struct stack* s);
void stack_push(struct stack* s, string* ch);
string* stack_pop(struct stack* s);
string* stack_top(struct stack* s);

#endif
