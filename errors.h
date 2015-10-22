enum compiler_error_type {
  NO_ERROR,
  ERR_LEX,
  ERR_SYN,
  ERR_SEM_UNDEF,
  ERR_SEM_TYPE,
  ERR_TYPE_INFERENCE,
  ERR_SEM_OTHER,
  ERR_RNT_NUM,
  ERR_RNT_NIN,
  ERR_RNT_ZER_DIV,
  ERR_RNT_OTHER,
  ERR_INT
};

struct compiler_error {
  int return_value;
  char* err_msg;
};

void throw_error(ERROR_CODE, const char *);
