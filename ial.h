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

typedef enum {
	SCOPE_FUNCTION, SCOPE_BLOCK
} ScopeType;

struct hash_table {
	int size;
	ScopeType scope_type;
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

// Global variable for the symbol table
// in which will be stored all scopes

int length(char* s);
char* substr(char* s, int i, int n);
char* concat(char* s1, char* s2);
int max(int a, int b);
char* sort(char* chars);
int find(char* s, char* search);

#endif
