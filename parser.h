#ifndef PARSER_H
#define PARSER_H

#include "string.h"

struct token
{
		enum token_type type;
		int line;

		string* data;
};

struct data* parser_run();
void parser_prepare(struct data*);

void get_scanned_token();

#endif
