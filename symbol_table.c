#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ial.h"
#include "errors.h"
#include "common.h"
#include "string.h"
#include "gc.h"
#include "stack.h"
#include "symbol_table.h"


// init zavolat na zacatku, drzet v GLOBALNI PROMENNE a predavat vsem volanim
struct symbol_table * init_table()
{
    struct symbol_table * table = malloc(sizeof(struct symbol_table));
    table->stack = malloc(sizeof(Stack));
    StackInit(table->stack);

    return table;
}


bool is_creatable(struct symbol_table * table, string *key) {
    Stack *current = StackTop(table->stack);
    void * symbol = get_item((struct hash_table*)current, key);
    return symbol == NULL;
}

// zavolat kdyz se zacne novy scope - v kazdem bloku instrukci (AST_LIST)!
void scope_start(struct symbol_table * symbol_table)
{
    // vytvorime novou hash tabulku a placnem ji na konec stacku
    struct hash_table * table = create_table(); //malloc(sizeof(struct hash_table));
    StackPush(symbol_table->stack, table);
}

// zavolat kdyz skonci scope - typicky po posledni zpracovane instrukci v AST_LIST
void scope_end(struct symbol_table * table)
{
    StackPop(table->stack);
}

// vrati to co sis ulozil se symbolem
// muzes si tam ukladat cokoliv, to je fuk, tabulku ani tohle to nezajima
void * get_symbol(struct symbol_table * table, string * key)
{
    Element * hash_table_carry = StackTopElement(table->stack);
    if (! hash_table_carry) {
        throw_error(CODE_ERROR_INTERNAL, "na zasobniku tabulek symbolu neni zadna!");
    }

    void * symbol = get_item(hash_table_carry->value, key);
    if (symbol) {
        return symbol;
    }
    // nenasli jsme, zaiterujem si
    while (hash_table_carry && hash_table_carry->next) {
        hash_table_carry = hash_table_carry->next;
        symbol = get_item(hash_table_carry->value, key);
        if (symbol) {
            return symbol;
        }
    }
    // nenasli jsme vubec
    return NULL;
}

// ulozi symbol
// value muze byt co chce
// krakonos klidne
// neni na vsechno potreba delat KrakonosPointer
void set_symbol(struct symbol_table * table, string * key, void * value)
{
    Element * hash_table_carry = StackTopElement(table->stack);
    if (! hash_table_carry) {
        throw_error(CODE_ERROR_INTERNAL, "na zasobniku tabulek symbolu neni zadna!");
    }

    add_item(hash_table_carry->value, key, value);
}
