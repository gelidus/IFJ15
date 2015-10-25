#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "string.h"
#include "gc.h"

void stack_init(Stack* s)
{
	s->head = NULL;
	s->node = NULL;
}

int stack_empty(Stack* s)
{
	return (s->head == NULL);
}

void stack_free(Stack* s)
{
	while(!stack_empty(s))
		stack_pop(s);

	s = NULL;
}

void stack_push(Stack* s, void* elem)
{
	if((s->node = (Element*) gc_malloc(sizeof(Element))) == NULL)
		return;

	s->node->value = elem;
	s->node->next = s->head;
	s->head = s->node;
}

void* stack_pop(Stack* s)
{
	if(s->head == NULL)
		return NULL;

	string* tmp = s->head->value;
	s->node = s->head;
	s->head = s->head->next;
	s->node = NULL;

	return tmp;
}

void* stack_top(Stack* s)
{
	if (s->head) {
		return s->head->value;
	} else
		return NULL;
}

