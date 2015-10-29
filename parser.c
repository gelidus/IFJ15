#include <stdbool.h>
#include "common.h"
#include "parser.h"
#include "gc.h"
#include "scanner.h"
#include "ast.h"

#define PRINT 1

bool program_body();
bool expect(enum lex_type t);
bool parse_program_body();
bool parse_statement(struct ast_node* instr, bool in_root);
bool parse_expression(struct ast_node* node);
bool parse_if(struct ast_node* node);
bool parse_return(struct ast_node* node);
bool token_empty();
bool token_if();
bool token_else();
bool token_left_par();
bool token_right_par();
bool token_left_brace();
bool token_right_brace();
bool token_semicolon();
bool token_return();
bool parse_datatype(enum ast_var_type* var_type);
bool token_variable(string* name);
bool token_datatype();
bool parse_var_creation(struct ast_node* node);


struct data* d;


// interfacova lahudka
void get_token()
{
    struct lexeme token = read_lexeme();
    // nejsem si uplne jistej, jestli to takhle bude fungovat!
    d->token = &token;
}


struct data* parser_run()
{
    if (PRINT) printf("\tparser: run");
    int result = expect(parse_program_body());
    if (! result) {
        d->error = CODE_ERROR_SYNTAX;
    }

    return d;
}

// alokace, nastaveni
void parser_prepare(struct data* data)
{
    if (PRINT) printf("\tparser: prepare");
    d = data;
    // inicializace stromu
    d->tree = ast_create_node();
}

bool no_errors()
{
    return d->error == CODE_OK;
}

// je dalsi token ten, ktery chci?
bool accept(enum lex_type t)
{
    if (PRINT) printf("\tparser: accepting token type %d\n", t);
    return t == d->token->type && no_errors();
}

// ocekavam na vstupu token, prijmu ho a prectu dalsi
bool expect(enum lex_type t)
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
    // doplnit, az bude jasne, co jak vypada
    if (token_datatype()) {
        EXPECT(parse_var_creation(node));
    } else if (accept(KW_IF)) {
        EXPECT(parse_if(node));
    } else if(accept(KW_RETURN)) {
        EXPECT(parse_return(node));
    // tyhle jsou uz fallback!
    } else if (token_empty()) {
        // prazdny statement je validni
        return true;
    } else if (accept(SEMICOLON)) {
        // je strednik vubec validni statement?
        return true;
    } else {
        // vsechno ostatni je pro me vyraz
        EXPECT(parse_expression(node));
    }

    EXPECT(no_errors());

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
        } else if (accept(RBR)) {
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

bool parse_expression(struct ast_node* node)
{
    if (PRINT) printf("\tparser: expression");
    // TODO: tady bude expression parser!
    return true;
}

bool parse_if(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing if");
    struct ast_node* condition = ast_create_node();
    struct ast_node* if_body   = ast_create_node();
    struct ast_node* else_body = ast_create_node();

    // if, zavorky, vyraz
    EXPECT(token_if());
    EXPECT(token_left_par());
    EXPECT(parse_expression(condition));
    EXPECT(token_right_par());
    EXPECT(no_errors());

    // if blok
    EXPECT(token_left_brace());
    EXPECT(parse_statement(if_body, false));
    EXPECT(token_right_brace());

    // else blok
    EXPECT(token_else());
    EXPECT(token_left_brace())
    EXPECT(parse_statement(else_body, false));
    EXPECT(token_right_brace());

    // poskladame
    node->type = AST_IF;
    node->d.condition = condition;
    node->left = if_body;
    node->right = else_body;

    return true;
}

bool parse_return(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing return");
    struct ast_node* expression = ast_create_node();
    EXPECT(token_return());
    EXPECT(parse_expression(expression));
    EXPECT(token_semicolon());

    //poskladame
    node->type = AST_RETURN;
    node->left = expression;
    return true;
}

bool parse_var_creation(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing variable creation!");
    struct ast_node* var = ast_create_node();
    struct ast_node* datatype = ast_create_node();
    enum ast_var_type* var_type = NULL;
    EXPECT(parse_datatype(var_type));
    node->type = AST_VAR_CREATION;
    datatype->var_type = *var_type;
    node->left = datatype;

    string* var_name = NULL;
    EXPECT(token_variable(var_name));
    var->d.string_data = var_name;
    var->type = AST_VAR;

    node->right = var;

    return true;
}

// TODO: mame vsechny z enumu?
bool token_empty()
{
    if (PRINT) printf("\tparser: empty\n");
    return expect(END_OF_FILE);
}

bool token_id(string* id)
{
    if (PRINT) printf("\tparser: ID\n");
    EXPECT(expect(IDENTIFIER));

    id = new_str(d->token->value.string);

    return true;
}

bool token_variable(string* var)
{
    if (PRINT) printf("\tparser: variable\n");
    EXPECT(expect(IDENTIFIER));

    var = new_str(d->token->value.string);

    return true;
}

bool token_datatype()
{
    if (PRINT) printf("\tparser: datatype\n");
    // tohle je chujovinaaaaaa
    if (! (accept(KW_INT) || accept(KW_STRING) || accept(KW_DOUBLE))) {
        d->error = CODE_ERROR_SYNTAX;
        return false;
    }

    return true;
}

bool parse_datatype(enum ast_var_type* var_type)
{
    if (PRINT) printf("\tparser: parsing datatype");
    // uhh
    if (accept(KW_INT)) {
        get_token();
        EXPECT(no_errors());
        *var_type = AST_VAR_INT;
        return true;
    } else if (accept(KW_STRING)) {
        // se s tim nebudu mrdat
        get_token();
        EXPECT(no_errors());
        *var_type = AST_VAR_STRING;
        return true;
    } else if (accept(KW_DOUBLE)) {
        get_token();
        EXPECT(no_errors());
        *var_type = AST_VAR_DOUBLE;
        return true;
    }

    // uhhhhh
    return false;
}

bool token_if()
{
    if (PRINT) printf("\tparser: if\n");
    return expect(KW_IF);
}

bool token_else()
{
    if (PRINT) printf("\tparser: else\n");
    return expect(KW_ELSE);
}

bool token_return()
{
    if (PRINT) printf("\tparser: return\n");
    return expect(KW_RETURN);
}

bool token_left_par()
{
    if (PRINT) printf("\tparser: left parenthesis\n");
    return expect(LPAR);
}

bool token_right_par()
{
    if (PRINT) printf("\tparser: right parenthesis\n");
    return expect(RPAR);
}

bool token_left_brace()
{
    if (PRINT) printf("\tparser: left brace\n");
    return expect(LBR);
}

bool token_right_brace()
{
    if (PRINT) printf("\tparser: right brace\n");
    return expect(RBR);
}

bool token_semicolon()
{
    if (PRINT) printf("\tparser: semicolon\n");
    return expect(SEMICOLON);
}

bool token_comma()
{
    if (PRINT) printf("\tparser: comma\n");
    return expect(COLON);
}
