#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "string.h"
#include "gc.h"

void StackInit(Stack *s)
{
	s->head = NULL;
	s->size = 0;
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
	Element* node;
	if((node = (Element*) gc_malloc(sizeof(Element))) == NULL)
		return;

	node->value = elem;
	node->next = s->head;
	s->head = node;

	s->size++;
}

void* StackPop(Stack *s)
{
	if(s->head == NULL)
		return NULL;

	Element* node = s->head;
	s->head = s->head->next;

	s->size--;

	return node->value;
}

void* StackTop(Stack *s)
{
	if (s->head) {
		return s->head->value;
	} else
		return NULL;
}


int StackSize(Stack *s) {
	return s->size;
}

Element *StackTopElement(Stack *s) {
	return s->head;
}
