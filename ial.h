#ifndef _IAL_H_
#define _IAL_H_

#include <stdbool.h>
#include <limits.h>
#include "string.h"
#include <stdio.h>
#include "common.h"
#include "ast.h"

// nova hashova tabulka co pisu ve ctvrtek pred odevzdanim
struct hash_item {
	string* key;
	void* value;
	struct hash_item* next;
};

struct hash_table {
	int size;
	struct hash_item** table;
};

struct hash_table * create_table();
int make_hash(struct hash_table * hashtable, string* key);
struct hash_item * make_item(string * key, void * value);
void add_item(struct hash_table * hashtable, string * key, void * value);
void * get_item(struct hash_table * hashtable, string * key);

/******************** HASH TABLE ********************/

#define MAX_HTSIZE 701
#define MAX_HTSTACK 107

/**
** Mozne typy.
*/
typedef enum {
	T_NIL = 0,
	T_NUM = 1,
	T_STR = 2,
	T_BOOL = 3,
	T_FUNC = 4
} Ttypes;

/**
** Pro zpracovani funkce.
*/
typedef struct FuncForm {
	bool def;			// Jestli byla funkce definovana.
	void *loc_hash;	// Lokalni hashovaci tabulka pro kazdou funkci.
	int param_count;	// Pocet parametru funkce.
	void *label;		// Navesti kde zacina funkce.
} FuncForm;

// Variable is linked list of Symbols which
// represents variable.
typedef struct Variable {
		struct ast_node* var;
		struct Variable* next;
} Variable;

// Scope is the registered scope of the function
// which contains linked list of available vairbales
// inside the function (scope)
// This variable provides the list scope by keeping the
// next scope variable.
typedef struct Scope {
		string* name;
		Variable* first;

		struct Scope * next;
} Scope;

// Global variable for the symbol table
// in which will be stored all scopes
Scope** SymbolTable;

// Hash table functions
unsigned long Hash(char* what);
void SymbolTableInit();
Variable* SymbolTableAddVariable(string* scope_name, struct ast_node* var);
Variable* SymbolTableFindVariable(string* scope_name, string* var_name);
Scope* SymbolTableFindScope(string *scope_name);
Scope* SymbolTableCreateScope(string *scope_name);
Variable* ScopeAddVariable(Scope* scope, struct ast_node* var);
Variable* ScopeFindVariable(Scope* scope, string* var_name);

int length(char* s);
char* substr(char* s, int i, int n);
char* concat(char* s1, char* s2);
int max(int a, int b);
char* sort(char* chars);
int find(char* s, char* search);

#endif
