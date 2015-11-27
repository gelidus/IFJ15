#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

#define DEBUG // enables DEBUGGING information
//#define UNIT_TEST

enum lex_type
{
    // + - * / =
  PLUS, //0
  MINUS, //1
  MULT, //2
  DIVIDE, //3
    // < > <= >= == != << >>
  LT, //4
  GT, //5
  LTE, //6
  GTE, //7
  NEQ, //8
  EQ, //9

  LPAR, //10
  RPAR, //11

  IDENTIFIER, //12
  NO_TYPE, //13

  EQUALS, //14
  //auto, cin, cout, double, else, for, if, int, return, string
  KW_AUTO, //15
  KW_CIN, //16
  KW_COUT, //17
  KW_DOUBLE, //18
  KW_ELSE, //19
  KW_FOR, //20
  KW_IF, //21
  KW_INT, //22
  KW_RETURN, //23
  KW_STRING, //24
  //literaly
  INTEGER, //25
  DOUBLE, //26
  STRING, //27

  INSOP, //28
  EXTOP,  //29
  // ( ) { }
  LBR, //30
  RBR, //31
  // ; ,
  SEMICOLON , //32
  COLON, //33
  END_OF_FILE //34
};

enum literal_type
{
    LITERAL_STRING,
    LITERAL_DOUBLE,
    LITERAL_INT
};

//structura lexemu
struct lexeme {
  enum lex_type type; //typ
  union {
    char *string;
    int integer;
    double real;
  } value; //hodnota ulozena ve value (pokud obsahuje)
};

struct data
{
    int error; // jsem se ulozi error, ten se dispatchne na nejvyssi urovni

    struct lexeme* token; // aktualni token

    FILE* file; // soubor, ktery bude interpretovan
    struct symbol_table* functions;
    struct symbol_table* vars;
    struct ast_node* tree;
};

// TODO: nema tam byt jeste jeden radek?
// nema tam byt jeste jeden radek?
// chyba "neni mozne odvodit datovy typ promenne"
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

enum ast_var_type
{
    AST_VAR_INT,
    AST_VAR_DOUBLE,
    AST_VAR_STRING,
    AST_VAR_NULL,  // pouze hodnota null
    AST_VAR_BOOL
};

/*
 * Makro, ktere ukonci funkci a vrati false, kdyz nastal error
 */
#define EXPECT(e)\
		if(!e) return false;

#endif
