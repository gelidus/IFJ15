#include <stdbool.h>
#include "common.h"
#include "parser.h"
#include "scanner.h"
#include "string.h"
#include "interpret.h"

#define PRINT  1

struct data d;


struct data parser_run()
{
    EXPECT(program_body());

    return d;
}

bool parse_function_call()
{
    EXPECT(token_id())

}

// alokace, nastaveni
void parser_prepare(struct data* data)
{
    d = data;
}

bool no_errors()
{
    return d->error = E_OK;
}

// je dalsi token ten, ktery chci?
bool accept(enum token_type t)
{
    if (PRINT) printf('\tparser: accepting token type %d\n', t);
    return t == d->token->type && no_errors();
}

// ocekavam na vstupu token, prijmu ho a prectu dalsi
bool expect(enum token_type t, ERROR_CODE error)
{
    if (PRINT) printf('\tparser: expecting token type %d\n', t);
    if (! accept(t)) {
        d->error = error;
        return false;
    }

    EXPECT(get_token());

    return true;
}

// TODO: mame vsechny z enumu?
bool token_empty()
{
    if (PRINT) printf('\tparser: empty\n');
    return except(T_THE_END);
}

bool token_id(string* id)
{
    if (PRINT) printf('\tparser: ID\n');
    EXPECT(except(T_ID));

    id = d->token->data;

    return true;
}

bool token_variable(string* var)
{
    if (PRINT) printf('\tparser: variable\n');
    EXPECT(except(T_VAR));

    var = d->token->data;

    return true;
}

bool token_if()
{
    if (PRINT) printf('\tparser: if\n');
    return except(T_IF);
}

bool token_else()
{
    if (PRINT) printf('\tparser: else\n');
    return except(T_ELSE);
}

bool token_while()
{
    if (PRINT) printf('\tparser: while\n');
    return except(T_WHILE);
}

bool token_return()
{
    if (PRINT) printf('\tparser: return\n');
    return except(T_RETURN);
}

bool token_left_par()
{
    if (PRINT) printf('\tparser: left parenthesis\n');
    return except(T_LPAR);
}

bool token_right_par()
{
    if (PRINT) printf('\tparser: right parenthesis\n');
    return except(T_RPAR);
}

bool token_left_brace()
{
    if (PRINT) printf('\tparser: left brace\n');
    return except(T_LBRACE);
}

bool token_right_brace()
{
    if (PRINT) printf('\tparser: right brace\n');
    return except(T_RBRACE);
}

bool token_semicolon()
{
    if (PRINT) printf('\tparser: semicolon\n');
    return except(T_SEMICOLON);
}

bool token_comma()
{
    if (PRINT) printf('\tparser: comma\n');
    return except(T_COMMA);
}
