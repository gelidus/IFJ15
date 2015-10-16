#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "string.h"


int length(string* s);
string substr(string* source, int offset, int length);
string concat(string* s1, string* s2);
int find(string* where, string* what);
string sort(string* s);

#endif
