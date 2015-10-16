#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "errors.h"

static FILE *input_file = NULL;

void set_input(char *input) {
	if(!(input_file = (!input) ? stdin : fopen(input, "r")))
		throw_error(ERR_INT, "cannot open input file");
}

int get_char() {
	if(!input_file) throw_error(ERR_INT, "cannot read from input");
	return getc(input_file);
}

int return_char(int c) {
	if(!input_file) throw_error(ERR_INT, "cannot read from input");
	return ungetc(c, input_file);	
}
