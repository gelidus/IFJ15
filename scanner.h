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

enum input_type {
  LETTER,
  DIGIT,
  WHITE_SPACE,
  UNDERSCORE,
  END,
  OTHERS
};

enum lex_type check_keyword(unsigned char *); //rozlyseni identifikatoru a kw
void read_input(); //nacteni znaku ze vstupu
void return_input(); //vraceni znaku na vstup nacteneho navic pri ukonceni lexemu

struct lexeme read_lexeme(void); // hlavni funkce

void free_token(struct lexeme *); //uvolneni struktury lexemu

#endif
