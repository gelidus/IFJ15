#ifndef PARSER_H
#define PARSER_H

#include "string.h"

struct data* parser_run();
void parser_prepare(struct data*);

void get_scanned_token();

#endif
