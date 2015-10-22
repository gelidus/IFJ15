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

//structura lexemu
struct lexeme {
  enum token_type type; //typ
  union {
    unsigned char *string;
    int integer;
    double real;
  } value; //hodnota ulozena ve value (pokud obsahuje)
};

enum token_type check_keyword(unsigned char *); //rozlyseni identifikatoru a kw
void read_input(); //nacteni znaku ze vstupu
void return_input(); //vraceni znaku na vstup nacteneho navic pri ukonceni lexemu

void get_token(); //hlavni funkce pro cteni vstupu a vraceni lexemu pro parser

void free_token(struct lexeme *); //uvolneni struktury lexemu

#endif
