#include <err.h>
#include "errors.h"

const struct compiler_error compiler_errors[] = {
  { 0, "no error" },
  { 1, "lexical error" },
  { 2, "syntactical error" },
  { 3, "semantic error - undefined/redefined function or variable" },
  { 4, "semantic error - uncompatible types for variables/parameters" },
  { 5, "semantic error - unable to find type of variable" },
  { 6, "semantic error - unspecific" },
  { 7, "runtime error - unable to read numerical value" },
  { 8, "runtime error - working with uninitialized memory" },
  { 9, "runtime error - division by zero" },
  { 10, "runtime error - unspecific" },
  { 99, "internal error"}
};

void throw_error(enum compiler_error_type type, const char *msg) {
  if(!msg) errx(compiler_errors[type].return_value, "%s", compiler_errors[type].err_msg);
  else errx(compiler_errors[type].return_value, "%s: %s", compiler_errors[type].err_msg, msg);
}
