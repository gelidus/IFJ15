#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "string.h"
#include "gc.h"

// Inicializace zasobniku
void stack_init(struct stack* s)
{
	s->head = NULL;
	s->node = NULL;
}

//kontrola
int stack_empty(struct stack* s)
{
	return (s->head == NULL);
}

// Uvolneni vsech prvku zasobniku pomoci pop
void stack_free(struct stack* s)
{
	while(!stack_empty(s))
		stack_pop(s);

	s = NULL;
}

// Pushnuti do zasobniku
void stack_push(struct stack* s, string* arr)
{
	if((s->node = (struct element*) gc_malloc(sizeof(struct element))) == NULL)
		return;

	s->node->value = arr;
	s->node->next = s->head;
	s->head = s->node;
}

// Popnuti ze zasobniku
string* stack_pop(struct stack* s)
{
	if(s->head == NULL)
		return NULL;

	string* tmp = s->head->value;
	s->node = s->head;
	s->head = s->head->next;
	s->node = NULL;

	return tmp;
}

// Ziskani vrcholu zasobniku
string* stack_top(struct stack* s)
{
	if (s->head) {
		return s->head->value;
	} else
		return NULL;
}
