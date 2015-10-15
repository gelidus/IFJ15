#include <stdbool.h>
#include "common.h"
#include "parser.h"
#include "scanner.h"
#include "string.h"
#include "interpret.h"
#include "stdlib.h"
#include "gc.h"

#define PRINT 1

bool program_body();
bool expect(enum token_type t);
bool parse_program_body();


struct data* d;


struct data* parser_run()
{
    int result = expect(parse_program_body());
    if (! result) {
        d->error = CODE_ERROR_SYNTAX;
    }

    return d;
}

bool parse_function_call()
{
    // EXPECT(token_id())

    return true;
}

// alokace, nastaveni
void parser_prepare(struct data* data)
{
    d = data;
    d->instructions = gc_malloc(sizeof(instruction));
}

bool no_errors()
{
    return d->error == CODE_OK;
}

// je dalsi token ten, ktery chci?
bool accept(enum token_type t)
{
    if (PRINT) printf("\tparser: accepting token type %d\n", t);
    return t == d->token->type && no_errors();
}

// ocekavam na vstupu token, prijmu ho a prectu dalsi
bool expect(enum token_type t)
{
    if (PRINT) printf("\tparser: expecting token type %d\n", t);
    if (! accept(t)) {
        d->error = CODE_ERROR_SYNTAX;
        return false;
    }

    get_token();
    EXPECT(no_errors());

    return true;
}

bool parse_program_block()
{
    return true;
}

bool parse_program_body()
{
    get_token();

    EXPECT(no_errors());

    EXPECT(parse_program_block());

    return true;
}


// TODO: mame vsechny z enumu?
bool token_empty()
{
    if (PRINT) printf("\tparser: empty\n");
    return expect(T_THE_END);
}

bool token_id(string* id)
{
    if (PRINT) printf("\tparser: ID\n");
    EXPECT(expect(T_ID));

    id = d->token->data;

    return true;
}

bool token_variable(string* var)
{
    if (PRINT) printf("\tparser: variable\n");
    EXPECT(expect(T_VAR));

    var = d->token->data;

    return true;
}

bool token_if()
{
    if (PRINT) printf("\tparser: if\n");
    return expect(T_IF);
}

bool token_else()
{
    if (PRINT) printf("\tparser: else\n");
    return expect(T_ELSE);
}

bool token_while()
{
    if (PRINT) printf("\tparser: while\n");
    return expect(T_WHILE);
}

bool token_return()
{
    if (PRINT) printf("\tparser: return\n");
    return expect(T_RETURN);
}

bool token_left_par()
{
    if (PRINT) printf("\tparser: left parenthesis\n");
    return expect(T_LPAR);
}

bool token_right_par()
{
    if (PRINT) printf("\tparser: right parenthesis\n");
    return expect(T_RPAR);
}

bool token_left_brace()
{
    if (PRINT) printf("\tparser: left brace\n");
    return expect(T_LBRACE);
}

bool token_right_brace()
{
    if (PRINT) printf("\tparser: right brace\n");
    return expect(T_RBRACE);
}

bool token_semicolon()
{
    if (PRINT) printf("\tparser: semicolon\n");
    return expect(T_SEMICOLON);
}

bool token_comma()
{
    if (PRINT) printf("\tparser: comma\n");
    return expect(T_COMMA);
}
