
#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "string.h"
#include "common.h"

struct ast_node* ast_create_node();
struct ast_list* ast_create_list();

bool ast_list_insert(struct ast_list* l, struct ast_node* n);
struct ast_list* ast_list_get_last(struct ast_list* l);
void ast_list_print(struct ast_list* l);
void ast_node_print(struct ast_node* n);


// ENUMY PRO abstraktni syntakticky strom
// po libosti upravujte
enum ast_node_type
{
    AST_STATEMENT = 1, // v sobe obsahuje cokoliv
    AST_ASSIGN = 2, // prirazeni do promenne
    AST_WHILE = 3, // cyklus while
    AST_RETURN = 4, // return
    AST_LITERAL = 5, // string, cisla, false, true, null
    AST_IF = 6, // ze by if?
    AST_VAR_CREATION = 7, //deklarace promenne

    AST_FUNCTION = 8, //vytvareni funkce
    AST_CALL = 9, //volani fce

    AST_LEFT_BRACKET = 10,
    AST_RIGHT_BRACKET = 11,
    AST_EXPRESSION = 12,

    AST_NONE = 13, // potrebny pre expression parser

    AST_VAR, //promenna
    AST_BINARY_OP, //binarni operace +-*/,
    AST_COUT,
    AST_CIN,
    AST_FOR,

    AST_FUNCTION_LIST,
    AST_FUNCTION_ARGUMENTS,
    AST_BODY,
};

enum ast_literal_type
{
    AST_LITERAL_FALSE,
    AST_LITERAL_TRUE,
    AST_LITERAL_NULL,
    AST_LITERAL_INT,
    AST_LITERAL_REAL,
    AST_LITERAL_STRING
};

enum ast_binary_op_type
{
    AST_BINARY_PLUS = 0,
    AST_BINARY_MINUS = 1,
    AST_BINARY_TIMES = 2,
    AST_BINARY_DIVIDE = 3,
    AST_BINARY_LESS = 4,
    AST_BINARY_MORE = 6,
    AST_BINARY_LESS_EQUALS = 7,
    AST_BINARY_MORE_EQUALS = 8,
    AST_BINARY_NOT_EQUALS = 9,
    AST_BINARY_EQUALS = 10,
};

union ast_node_data
{
    enum ast_binary_op_type binary; // typ binarni operace
    string* string_data; // nazev fce, nebo promenne, text
    double numeric_data;
    struct ast_node* condition; //pro podminku u if
    struct ast_list* list; // pro uchovavani agumentu, statement body, function body, if body, else body..
};

// po libosti upravujte, kdyz vam neco nesedi!
struct ast_node
{
    enum ast_node_type type;

    union ast_node_data d;

    enum ast_var_type var_type;
    enum ast_literal_type literal; // typ konstanty

    struct ast_node* left;
    struct ast_node* right;
};

// seznam instrukci
struct ast_list
{
    struct ast_node* elem;
    struct ast_list* next;
};

#endif
