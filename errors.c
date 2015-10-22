#include <err.h>
#include "errors.h"

const struct compiler_error compiler_errors[] = {
  { 0, "no error" },
  { 1, "lexical error" },
  { 2, "syntactical error" },
  { 3, "semantic error - undefined/redefined function or variable" },
  { 4, "semantic error - uncompatible types for variables/parameters" },
  { 5, "semantic error - unspecific" },
  { 6, "runtime error - unable to read numerical value" },
  { 7, "runtime error - working with uninitialized memory" },
  { 8, "runtime error - division by zero" },
  { 9, "runtime error - unspecific" },
  { 99, "internal error"},
  { 1000, "parse end"}
};

void throw_error(ERROR_CODE type, const char *msg) {
  if(!msg) errx(compiler_errors[type].return_value, "%s", compiler_errors[type].err_msg);
  else errx(compiler_errors[type].return_value, "%s: %s", compiler_errors[type].err_msg, msg);
}
