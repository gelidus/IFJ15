#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "input.h"
#include "errors.h"
#include "common.h"

#define CHUNK 256

enum lex_type
{
  NO_TYPE,
  IDENTIFIER,
  KW_AUTO, KW_CIN, KW_COUT, KW_DOUBLE, KW_ELSE, KW_FOR, KW_IF, KW_INT, KW_RETURN, KW_STRING, //auto, cin, cout, double, else, for, if, int, return, string
  INTEGER, DOUBLE, STRING, //literaly
  PLUS, MINUS, MULT, DIVIDE, EQUALS, // + - * / =
  LT, GT, LTE, GTE, EQ, NEQ, INSOP, EXTOP, // < > <= >= == != << >>
  LPAR, RPAR, LBR, RBR, // ( ) { }
  SEMICOLON, COLON, // ; ,
  END_OF_FILE
};

enum input_type {
  LETTER,
  DIGIT,
  WHITE_SPACE,
  UNDERSCORE,
  END,
  OTHERS
};

//structura lexemu
struct lexeme {
  enum lex_type type; //typ
  union {
    unsigned char *string;
    int integer;
    double real;
  } value; //hodnota ulozena ve value (pokud obsahuje)
};

enum lex_type check_keyword(unsigned char *); //rozlyseni identifikatoru a kw
void read_input(); //nacteni znaku ze vstupu
void return_input(); //vraceni znaku na vstup nacteneho navic pri ukonceni lexemu

struct lexeme read_lexeme(void); // hlavni funkce

void free_token(struct lexeme *); //uvolneni struktury lexemu

#endif
