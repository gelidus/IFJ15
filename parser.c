#include <stdbool.h>
#include "common.h"
#include "parser.h"
#include "gc.h"
#include "scanner.h"
#include "ast.h"

#define PRINT 1

bool program_body();
bool expect(enum token_type t);
bool parse_program_body();
bool parse_statement(struct ast_node* instr, bool in_root);
bool token_empty();


struct data* d;


// interfacova lahudka
void get_scanned_token()
{
    struct lexeme lex = get_token();
    // nasetovat do datove struktury veci z lex struktury
}


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
    // inicializace stromu
    d->tree = ast_create_node();
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

bool parse_statement(struct ast_node* node, bool in_root)
{
    // create var
    // while
    // if
    // return
    // fn_call
    //function declaration
    // dopln kod

    return true;
}

bool parse_program_block(struct ast_node* node, bool in_root)
{
    // priprav seznam instrukci bloku
    node->d.list = ast_create_list();

    struct ast_node* new_node;
    while(true) {
        // posledni token ale nejsme v bloku?
        if (!in_root && token_empty()) {
            d->error = CODE_ERROR_SYNTAX;
            return false;
        // posledni token v mainu programu je poslednim tokenem
        } else if (in_root && token_empty()) {
            break;
        // zavrena zavorka v bloku je konec bloku
        } else if (accept(T_RBRACE)) {
            break;
        }

        // soucasna instrukce
        new_node = ast_create_node();

        EXPECT(parse_statement(new_node, in_root))

        // vloz do seznamu
        ast_list_insert(node->d.list, new_node);

        // mozna jde prepsat?
        if (token_empty()) {
            break;
        }
    }

    return true;
}

bool parse_program_body()
{
    get_token();

    EXPECT(no_errors());

    EXPECT(parse_program_block(d->tree, true));

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
