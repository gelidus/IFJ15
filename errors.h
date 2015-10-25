#ifndef ERRORS_H
#define ERRORS_H

#include "common.h"
struct compiler_error {
  int return_value;
  char* err_msg;
};

void throw_error(ERROR_CODE type, const char* err);

#endif
