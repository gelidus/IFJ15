#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ial.h"
#include "stack.h"

void * get_symbol(struct symbol_table * table);
void set_symbol(struct symbol_table * table);
struct symbol_table * init_table();

void scope_start(struct symbol_table * table);
void scope_end(struct symbol_table * table, string * key);

struct symbol_table {
    Stack * stack;
};
