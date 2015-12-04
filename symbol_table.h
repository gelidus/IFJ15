#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ial.h"
#include "stack.h"

void * get_symbol(struct symbol_table * table, string * key);
void set_symbol(struct symbol_table * table, string * key, void * value);
struct symbol_table * init_table();

// is_creatable only looks into the current
// scope for the value to determine if it is already
// taken or not
bool is_creatable(struct symbol_table * table, string* key);

void scope_start(struct symbol_table * table);
void scope_end(struct symbol_table * table);

struct symbol_table {
    Stack * stack;
};

#endif
