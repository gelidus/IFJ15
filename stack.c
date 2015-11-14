#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "string.h"
#include "gc.h"

void StackInit(Stack *s)
{
	s->head = NULL;
	s->node = NULL;
}

int StackEmpty(Stack *s)
{
	return (s->head == NULL);
}

void StackFree(Stack *s)
{
	while(!StackEmpty(s))
		StackPop(s);

	s = NULL;
}

void StackPush(Stack *s, void *elem)
{
	if((s->node = (Element*) gc_malloc(sizeof(Element))) == NULL)
		return;

	s->node->value = elem;
	s->node->next = s->head;
	s->head = s->node;
}

Element* StackPop(Stack *s)
{
	if(s->head == NULL)
		return NULL;

	Element* tmp = s->head->value;
	s->node = s->head;
	s->head = s->head->next;
	s->node = NULL;

	return tmp;
}

Element* StackTop(Stack *s)
{
	if (s->head) {
		return s->head;
	} else
		return NULL;
}

