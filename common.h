#include <stdio.h>
#include <string.h>
#include "string.h"

#ifndef COMMON_H
#define COMMON_H

// postupne doplnit
enum token_type {
    T_ID,// 0
    T_VAR,// 1
    T_STRING, // 3 ukladani stringu
    T_FUNCTION, // 4
    T_IF, // 5
    T_ELSE, // 6
    T_WHILE, // 7
    T_RETURN, // 8
    T_LPAR, // 9 (
    T_RPAR, // 10 )
    T_LBRACE, // 11 {
    T_RBRACE, // 12}
    T_NOT, // 13
    T_PLUS, // 14
    T_MINUS, // 15
    T_MULTIPLY, // 16
    T_DIVIDE, // 17
    T_LESS, // 18
    T_MORE, // 19
    T_LESSOREQ, // 20
    T_MOREOREQ, // 21
    T_EQUALS, // 22 ==
    T_NOTEQUAL, // 23 !=
    T_STRICTNOTEQ, // !25 ==

    T_ASSIGN, // 26 =
    T_TRUE, // 27
    T_FALSE, // 28
    T_NULL, //29
    T_COMMA, // 30
    T_SEMICOLON, // 31
    T_CONCATENATE, // 32
    T_THE_END // 33
};
struct token
{
    enum token_type type;
    int line;

    string* data;
};
// nejsem si jistej, jestli to je potreba
struct symbol_table_data
{
	double numeric_data;
	string* string_data;
	enum ast_literal_type literal_type;
};
struct symbol_table
{
	//klic a nebo nazev promenne
	char* key;

	//typ obsahu
	enum ast_var_type var_type;

	//datova struktura
	struct symbol_table_data d;

	//v pripade funkce pro uchovani posloupnosti prikazu
	struct ast_node* statement;
	struct ast_list* params;

	//leva a prava podvetev
	struct symbol_table *left;
	struct symbol_table *right;
};

struct data
{
    int error; // jsem se ulozi error, ten se dispatchne na nejvyssi urovni

    struct token* token; // aktualni token

    FILE* file; // soubor, ktery bude interpretovan
    struct symbol_table* functions;
    struct symbol_table* vars;
    struct ast_node* tree;
};

typedef enum ERROR_CODES {
    CODE_OK = 0,
    CODE_ERROR_LEX = 1,
    CODE_ERROR_SYNTAX = 2,
    CODE_ERROR_SEMANTIC = 3,
    CODE_ERROR_COMPATIBILITY = 4,
    CODE_ERROR_SEMANT_OTHER = 5,
    CODE_ERROR_NUMBER_IN = 6,
    CODE_ERROR_UNINITIALIZED_ID = 7,
    CODE_ERROR_RUNTIME_DIV_BY_0 = 8,
    CODE_ERROR_RUNTIME_OTHER = 9,
    CODE_ERROR_INTERNAL = 99,

    CODE_PARSE_END = 1000
} ERROR_CODE;

/*
 * Makro, ktere ukonci funkci a vrati false, kdyz nastal error
 */
#define EXPECT(e)\
		if(!e) return false;

// ocekava validitu
#define EXPECT_VALIDITY(d)\
        if(d->error != 0) return false;

#endif
