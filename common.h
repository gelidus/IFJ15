#ifndef COMMON_H
#define COMMON_H

#define DEBUG true

#include <stdio.h>
#include <stdlib.h>

enum lex_type
{
  NO_TYPE,
  IDENTIFIER,
  KW_AUTO, KW_CIN, KW_COUT, KW_DOUBLE, KW_ELSE, KW_FOR, KW_IF, KW_INT, KW_RETURN, KW_STRING, //auto, cin, cout, double, else, for, if, int, return, string
  KW_WHILE, // while, netusim, jestli tu ma byt.. ?
  INTEGER, DOUBLE, STRING, //literaly
  PLUS, MINUS, MULT, DIVIDE, EQUALS, // + - * / =
  LT, GT, LTE, GTE, EQ, NEQ, INSOP, EXTOP, // < > <= >= == != << >>
  LPAR, RPAR, LBR, RBR, // ( ) { }
  SEMICOLON, COLON, // ; ,
  END_OF_FILE
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
    unsigned char *string;
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

/*
 * Makro, ktere ukonci funkci a vrati false, kdyz nastal error
 */
#define EXPECT(e)\
		if(!e) return false;

// ocekava validitu
// a taky ocekava, ze se datova struktura jmenuje d
// kdyz se d nebude jmenovat, tak to nebude fungovat !
#define EXPECT_VALIDITY(d)\
        if(d->error != 0) return false;

#endif
