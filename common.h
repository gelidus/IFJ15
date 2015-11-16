#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

#define DEBUG // enables DEBUGGING information
//#define UNIT_TEST

enum lex_type
{
    // + - * / =
  PLUS, //15
  MINUS, //16
  MULT, //17
  DIVIDE, //18
    // < > <= >= == != << >>
  LT, //20
  GT, //21
  LTE, //22
  GTE, //23
  NEQ, //25
  EQ, //24

  LPAR, //28
  RPAR, //29

  IDENTIFIER, //1
  NO_TYPE, //0

  EQUALS, //19
  //auto, cin, cout, double, else, for, if, int, return, string
  KW_AUTO, //2
  KW_CIN, //3
  KW_COUT, //4
  KW_DOUBLE, //5
  KW_ELSE, //6
  KW_FOR, //7
  KW_IF, //8
  KW_INT, //9
  KW_RETURN, //10
  KW_STRING, //11
  //literaly
  INTEGER, //12
  DOUBLE, //13
  STRING, //14

  INSOP, //26
  EXTOP,  //27
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

// doplnim postupne
enum instruction_type {
    INST_ASSIGN,
    // odlisna od prirazeni!
    INST_VAR_CREATE,
    INST_WHILE,
    INST_IF,
    INST_FN_CALL,
    INST_RETURN,
    INST_COUT,
    INST_CIN,
    INST_FN_DEFINITION,
    INST_FOR,
    // binarni operace
    INST_BIN_OP,
};

typedef struct instruction {
    enum instruction_type type;
    struct symbol_table *source1;
    struct symbol_table *source2;
    void *destination;

    struct instruction *next;
} instruction;

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
    AST_VAR_NULL  // pouze hodnota null
};

/*
 * Makro, ktere ukonci funkci a vrati false, kdyz nastal error
 */
#define EXPECT(e)\
		if(!e) return false;

#endif
