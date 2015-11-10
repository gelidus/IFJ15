#ifndef _IAL_H_
#define _IAL_H_

#include <stdbool.h>
# include <limits.h>
#include "string.h"
# include <stdio.h>
#include "common.h"

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
		struct Variable* next;
} Variable;

// Scope is the registered scope of the function
// which contains linked list of available vairbales
// inside the function (scope)
typedef struct {
		string* name;
		Variable* first;
} Scope;

// Global variable for the symbol table
// in which will be stored all scopes
Scope* SymbolTable;

// Hash table functions
unsigned long Hash(unsigned char* what);
void HashTableInit();
void HashTableAddVariable(string* scope, string* var_name, enum ast_var_type type);
Variable* HashTableFindVariable(string* scope, string* var_name);

int length(char* s);
char* substr(char* s, int i, int n);
char* concat(char* s1, char* s2);
int max(int a, int b);
char* sort(char* chars);
int find(char* s, char* search);

#endif
