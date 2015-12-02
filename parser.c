#include <stdbool.h>
#include "common.h"
#include "parser.h"
#include "gc.h"
#include "scanner.h"
#include "ast.h"
#include "stack.h"

#define PRINT 1

bool expect(enum lex_type t);
bool accept(enum lex_type t);

bool token_empty();
bool token_if();
bool token_else();
bool token_left_par();
bool token_right_par();
bool token_left_brace();
bool token_right_brace();
bool token_semicolon();
bool token_return();
bool token_comma();
bool token_extop();
bool token_insop();
bool token_datatype();
bool token_cout();
bool token_cin();
bool token_for();
bool token_equals();

bool parse_decision(struct ast_node* node);
bool parse_id(string** name);
bool parse_datatype(enum ast_var_type* var_type);
bool parse_var_creation(struct ast_node* node);
bool parse_function_definition(struct ast_node* node);
bool parse_function_arguments(struct ast_node* node);
bool parse_cout(struct ast_node* node);
bool parse_cin(struct ast_node* cin);
bool parse_program_body();
bool parse_statement(struct ast_node* instr);
bool parse_expression(struct ast_node* node);
bool parse_if(struct ast_node* node);
bool parse_return(struct ast_node* node);
bool parse_for(struct ast_node* node);
bool parse_assign(struct ast_node* node);
bool handle_id(struct ast_node* node);

struct data* d;


// interfacova lahudka
void get_token()
{
    struct lexeme token = read_lexeme();
    // takhle je to potreba delat, protoze jinak se ccko z tech pointeru zblazni
    d->token = (struct lexeme*)malloc(sizeof(struct lexeme));
    d->token->type = token.type;
    d->token->value = token.value;
    if (PRINT) printf("\tLEXER: got token of type %i\n", d->token->type);
}


struct data* parser_run()
{
    if (PRINT) printf("\tparser: run\n");
    int result = parse_program_body();
    if (PRINT) printf("\tparser: run finished\n");
    if (! result) {
        if (PRINT) printf("\tparser: SETTING ERROR VALUE\n");
        d->error = CODE_ERROR_SYNTAX;
    }

    return d;
}

// alokace, nastaveni
void parser_prepare(struct data* data)
{
    if (PRINT) printf("\tparser: prepare\n");
    d = data;
    // inicializace stromu
    d->tree = ast_create_node();
    d->tree->type = AST_FUNCTION_LIST;

    d->tree->d.list = ast_create_list();
}

bool no_errors()
{
    if (d->error != CODE_OK) {
        if (PRINT) printf("\tparser: no_errors failed since error no is %d\n", d->error);
    }

    return d->error == CODE_OK;
}

// je dalsi token ten, ktery chci?
bool accept(enum lex_type t)
{
    if (PRINT) printf("\tparser: accepting token type %d; actual type is now %d\n", t, d->token->type);
    return ((t == d->token->type) && no_errors());
}

// ocekavam na vstupu token, prijmu ho a prectu dalsi
bool expect(enum lex_type t)
{
    if (PRINT) printf("\tparser: expecting token type %d\n", t);
    if (! accept(t)) {
        if (PRINT) printf("\tparser: SETTING ERROR VALUE\n");
        d->error = CODE_ERROR_SYNTAX;
        return false;
    }

    get_token();
    EXPECT(no_errors());

    return true;
}

bool parse_statement(struct ast_node* node)
{
    // doplnit, az bude jasne, co jak vypada
    if (PRINT) printf("\tparser: handling statement\n");
    if (token_datatype()) {
        EXPECT(parse_var_creation(node));
        EXPECT(token_semicolon());
    } else if (accept(KW_IF)) {
        EXPECT(parse_if(node));
    } else if(accept(KW_RETURN)) {
        EXPECT(parse_return(node));
    } else if (accept(KW_COUT)) {
        EXPECT(parse_cout(node));
    } else if (accept(KW_CIN)) {
        EXPECT(parse_cin(node));
    } else if (accept(KW_FOR)) {
        EXPECT(parse_for(node));
    } else if (accept(IDENTIFIER)) {
        EXPECT(handle_id(node));
        EXPECT(token_semicolon());
    // tyhle jsou uz fallback!
    } else if (token_empty()) {
        // prazdny statement je validni
        node->type = AST_NONE;
        return true;
    } else if (accept(SEMICOLON)) {
        EXPECT(token_semicolon());
        node->type = AST_NONE;
    } else {
        // vsechno ostatni je pro me vyraz
        EXPECT(parse_expression(node));
    }

    EXPECT(no_errors());

    return true;
}

bool parse_decision(struct ast_node* node)
{
    if (PRINT) printf("\tparser: handling decision\n");

    struct ast_node* new_node;
    while(true) {
        // posledni token v rootu je poslednim tokenem
        if (token_empty()) {
            if (PRINT) printf("\tparser: eof detected\n");
            break;
        }

        new_node = ast_create_node();
        // v rootu jsou jen deklarace funkci
        EXPECT(parse_function_definition(new_node));
        if (PRINT) printf("\tparser: finished parsing function definition\n");
        ast_list_insert(node->d.list, new_node);

        // tohle by asi chtelo prepsat
        if (token_empty()) {
            if (PRINT) printf("\tparser: eof detected\n");
            break;
        }
    }

    if (PRINT) printf("\tparser: parsed decision\n");

    return true;
}

bool parse_program_block(struct ast_node* node)
{
    if (PRINT) printf("\tparser: handling program block\n");
    // priprav seznam instrukci bloku
    node->d.list = ast_create_list();

    struct ast_node* new_node;
    while(true) {
        // posledni token ale nejsme v bloku?
        if (token_empty()) {
            if (PRINT) printf("\tparser: SETTING ERROR VALUE\n");
            d->error = CODE_ERROR_SYNTAX;
            return false;
        // zavrena zavorka v bloku je konec bloku
        } else if (accept(RBR)) {
            break;
        }

        // soucasna instrukce
        new_node = ast_create_node();
        // vsude to budou statementy
        EXPECT(parse_statement(new_node))

        // vloz do seznamu
        ast_list_insert(node->d.list, new_node);
    }

    return true;
}

bool parse_program_body()
{
    get_token();

    EXPECT(no_errors());

    EXPECT(parse_decision(d->tree));

    if (PRINT) printf("\tparser: parsed program body\n");

    return true;
}

bool parse_for(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing for\n");

    struct ast_node* first_field = ast_create_node();
    struct ast_node* second_field = ast_create_node();
    struct ast_node* third_field = ast_create_node();
    struct ast_node* block = ast_create_node();

    EXPECT(token_for());
    // podminky
    EXPECT(token_left_par());
    // prvni vyraz muze zacinat var creation
    if (token_datatype()) {
        parse_var_creation(first_field);
    // nebo je to jen vyraz
    } else {
        EXPECT(parse_assign(first_field));
    }
    EXPECT(token_semicolon());
    EXPECT(parse_expression(second_field))
    EXPECT(token_semicolon());
    // posledni dilecek je assign - tedy metoda handle_id (co resi assign a fn cally)
    EXPECT(handle_id(third_field));
    EXPECT(token_right_par());

    EXPECT(token_left_brace())
    EXPECT(parse_program_block(block));
    EXPECT(token_right_brace());

    node->d.list = ast_create_list();
    // povkladame
    ast_list_insert(node->d.list, first_field);
    ast_list_insert(node->d.list, second_field);
    ast_list_insert(node->d.list, third_field);

    node->type = AST_FOR;
    node->left = block;

    return true;
}

bool parse_assign(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing assign inside for!\n");
    struct ast_node* id = ast_create_node();
    string* var_name = NULL;
    EXPECT(parse_id(&var_name));
    id->d.string_data = var_name;
    id->type = AST_VAR;

    node->type = AST_ASSIGN;
    node->left = id;

    expect(EQUALS);

    struct ast_node* expr = ast_create_node();
    EXPECT(parse_expression(expr));
    node->right = expr;

    return true;
}

bool parse_cout(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing cout\n");

    node->d.list = ast_create_list();

    EXPECT(token_cout());

    while (! accept(SEMICOLON)) {
        EXPECT(token_insop());
        struct ast_node* expression = ast_create_node();
        EXPECT(parse_expression(expression));

        if (PRINT) printf("\tparser: COUT GOT ANOTHER\n");
        ast_list_insert(node->d.list, expression);
    }
    node->type = AST_COUT;

    return true;
}

bool parse_cin(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing cin\n");

    node->d.list = ast_create_list();

    EXPECT(token_cin());

    while (! accept(SEMICOLON)) {
        EXPECT(token_extop());
        string* var = NULL;
        EXPECT(parse_id(&var));
        struct ast_node* variable = ast_create_node();
        variable->type = AST_VAR;
        variable->d.string_data = var;
        ast_list_insert(node->d.list, variable);
    }
    node->type = AST_CIN;

    return true;
}

bool parse_function_definition(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing function definition\n");

    struct ast_node* arguments = ast_create_node();
    struct ast_node* body = ast_create_node();
    body->type = AST_BODY;

    enum ast_var_type* var_type = malloc(sizeof(enum ast_var_type));
    EXPECT(parse_datatype(var_type));

    string* function_name = NULL;
    EXPECT(parse_id(&function_name));

    EXPECT(parse_function_arguments(arguments));

    EXPECT(token_left_brace());
    EXPECT(parse_program_block(body));
    EXPECT(token_right_brace());

    // poskladame
    node->type = AST_FUNCTION;
    node->d.string_data = function_name;
    node->left = arguments;
    node->right = body;

    return true;
}

bool parse_function_arguments(struct ast_node* node)
{
    node->d.list = ast_create_list();
    node->type = AST_FUNCTION_ARGUMENTS;

    EXPECT(token_left_par());
    string* var_name = NULL;
    // dokud argumenty neskonci
    while (! accept(RPAR)) {
        if ( PRINT ) printf("\tparser: function argument parsing\n");
        // nejdriv datatyp, potom promenna
        enum ast_var_type* var_type = malloc(sizeof(enum ast_var_type));
        EXPECT(parse_datatype(var_type));
        EXPECT(parse_id(&var_name));
        // vytvorime promennou
        struct ast_node* variable = ast_create_node();
        variable->type = AST_VAR;
        variable->d.string_data = var_name;
        // zalozime do seznamu argumentu
        ast_list_insert(node->d.list, variable);
        if (! accept(RPAR)) {
            EXPECT(token_comma());
        }
    }

    EXPECT(token_right_par());

    return true;

}

#define OPERATORS 14
const char PrecendenceTable[OPERATORS][OPERATORS] = {
    /////   +    -    *    /    <    >   <=   >=   <>   ==    (    )   ID    $
    /*+*/ {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /*-*/ {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /***/ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /*/*/ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /*<*/ {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /*>*/ {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /*<=*/{'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /*>=*/{'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /*<>*/{'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /*= */{'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},
    /*(*/ {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<',  0 },
    /*)*/ {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 0  , '>',  0 , '>'},
    /*ID*/{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 0  , '>',  0 , '>'},
    /*$*/ {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<' , 0 , '<', '$'}
};

bool IsExpressionNode(struct ast_node* node) {
    switch(node->type) {
        case AST_EXPRESSION:
        case AST_BINARY_OP:
        case AST_VAR:
        case AST_LITERAL:
            return true;
        default:
            return false;
    }
}

bool IsOperatorNode(struct ast_node* node) {
    if (node == NULL) {
        return false;
    }

    switch (node->type) {
        case AST_LEFT_BRACKET:
        case AST_RIGHT_BRACKET:
            return true;
        case AST_BINARY_OP:
            return node->left == NULL && node->right == NULL;
        default:
            return false;
    }
}

// GetASTNodeFromToken converts given lexeme into the ast_node
// with the corresponding values.
// Warning: This function is only for the expression. If it's
// populated with the non-expression token, it will return
// null instead
struct ast_node* GetASTNodeFromToken(struct lexeme* lex) {
    struct ast_node* node = ast_create_node();

    switch(lex->type) {
        case DOUBLE:
            node->type = AST_LITERAL;
            node->literal = AST_LITERAL_REAL;
            node->d.numeric_data = lex->value.real;
        break;
        case INTEGER:
            node->type = AST_LITERAL;
            node->literal = AST_LITERAL_INT;
            node->d.numeric_data = lex->value.integer;
        break;
        case STRING:
            node->type = AST_LITERAL;
            node->literal = AST_LITERAL_STRING;
            node->d.string_data = new_str(lex->value.string);
        break;
        case PLUS:
            node->type = AST_BINARY_OP;
            node->d.binary = AST_BINARY_PLUS;
        break;
        case MINUS:
            node->type = AST_BINARY_OP;
            node->d.binary = AST_BINARY_MINUS;
        break;
        case MULT:
            node->type = AST_BINARY_OP;
            node->d.binary = AST_BINARY_TIMES;
        break;
        case LPAR:
            node->type = AST_LEFT_BRACKET;
        break;
        case RPAR:
            node->type = AST_RIGHT_BRACKET;
        break;
        case DIVIDE:
            node->type = AST_BINARY_OP;
            node->d.binary = AST_BINARY_DIVIDE;
        break;
        case LT:
            node->type = AST_BINARY_OP;
            node->d.binary = AST_BINARY_LESS;
        break;
        case GT:
            node->type = AST_BINARY_OP;
            node->d.binary = AST_BINARY_MORE;
        break;
        case LTE:
            node->type = AST_BINARY_OP;
            node->d.binary = AST_BINARY_LESS_EQUALS;
        break;
        case GTE:
            node->type = AST_BINARY_OP;
            node->d.binary = AST_BINARY_MORE_EQUALS;
        break;

        case IDENTIFIER:
            node->type = AST_VAR;
            node->d.string_data = new_str(lex->value.string);
        break;

        default:
            return NULL; // token is not from the expression tokens
    }

    return node;
}

// GetASTNodePrecendenceValue will return the exact precendence
// value from the ast_node. This function is needed because of
// the ast_node subtyping
int GetASTNodePrecendenceValue(struct ast_node* node) {
    if (node == NULL) {
        return AST_NONE;
    }

    switch(node->type) {
        case AST_BINARY_OP:
            return node->d.binary;
        case AST_LITERAL:
        case AST_VAR:
            return AST_EXPRESSION;
        default:
            return node->type;
    }
}

// GetPrecendence accepts two ast_node pointers: stack ast node and
// matching next node on the input. This will return the precendence
// character from the precendence table based on the types of thse
// two ast_node types.
char GetPrecendence(struct ast_node* stacked, struct ast_node* next) {
    return PrecendenceTable[GetASTNodePrecendenceValue(stacked)][GetASTNodePrecendenceValue(next)];
}

// GetStackTopOperator returns top-most operator from the
// given stack of ast_node. This must be implemented because
// of the precendence matching mechanism and due to missing
// output stack (as we are creating ast tree instead)
struct ast_node* GetStackTopOperator(Stack *stack) {

    // get the top element from the stack so we can interate over it
    Element *el = StackTopElement(stack);

    // find the top operator on the stack
    while(el != NULL) {
        struct ast_node* node = el->value;
        if (IsOperatorNode(node)) {
            return node;
        }

        el = el->next;
    }

    return NULL; // no type
}

// Node passed to this function will automacially become
// expression node, which will store the expression in the
// left leaf. This is beacause of empty expressions parsing
bool parse_expression(struct ast_node* node) {
    if ( PRINT ) printf("\tparser: parsing expression\n");

    node->type = AST_EXPRESSION;

    Stack stack;
    StackInit(&stack);

    enum ast_node_type stackType = AST_NONE;
    struct ast_node* source1 = NULL, *source2 = NULL, *result = NULL, *next_node = NULL;

    // get the first token of the expression and convert it
    //get_token(); - we already have the first token (was not returned)
    next_node = GetASTNodeFromToken(d->token);

    do { // until $ on all stacks
        // next_node is not in the
        if (next_node == NULL && StackEmpty(&stack)) {
            return true;
        }

        if (GetStackTopOperator(&stack) == NULL && accept(RPAR)) {
            if (PRINT) printf("\texpr parser: found right par next_node is null\n");
            next_node = NULL;
        }

        char precendenceCharacter = GetPrecendence(GetStackTopOperator(&stack), next_node);

        switch(precendenceCharacter) {
            case '=':
            case '<': {
                StackPush(&stack, next_node);

                // prepare the next token of the expression
                get_token(); next_node = GetASTNodeFromToken(d->token);
                break;
            }
            case '>': {
                struct ast_node* current = StackPop(&stack);

                if (current != NULL && current->type != AST_RIGHT_BRACKET) {
                    //// E -> E1 op E2 ////

                    // get second source
                    source2 = current;

                    // pop operator from the stack
                    result = StackPop(&stack);

                    // get first source
                    source1 = StackPop(&stack);

                    // add leafs to the result
                    result->left = source1;
                    result->right = source2;

                } else if (current != NULL) {
                    //// E -> (E) ////
                    result = StackPop(&stack);

                    current = StackPop(&stack);
                    // last bracket must be left
                    if (current == NULL || current->type != AST_LEFT_BRACKET) {
                        throw_error(CODE_ERROR_SYNTAX, "");
                    }
                } else {
                    throw_error(CODE_ERROR_SYNTAX, "");
                }

                StackPush(&stack, result);
                break;
            }
            case '$':
                if (StackTop(&stack) == NULL || StackSize(&stack) > 1) {
                    throw_error(CODE_ERROR_SYNTAX, "");
                }

                // save the result to the left node of the expression
                if (IsExpressionNode(StackTop(&stack))) {
                    node->left = StackPop(&stack);
                }

            break;


            default:
                throw_error(CODE_ERROR_SYNTAX, "");
            break;
        }

    } while(!StackEmpty(&stack) || next_node != NULL);


    if ( PRINT ) printf("\tparser: finished parsing expression\n");

    return true;
}

bool parse_if(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing if\n");
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
    EXPECT(parse_program_block(if_body));
    EXPECT(token_right_brace());

    // else blok
    EXPECT(token_else());
    EXPECT(token_left_brace())
    EXPECT(parse_program_block(else_body));
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
    if (PRINT) printf("\tparser: parsing return\n");
    struct ast_node* expression = ast_create_node();
    EXPECT(token_return());
    EXPECT(parse_expression(expression));
    EXPECT(token_semicolon());

    //poskladame
    node->type = AST_RETURN;
    node->left = expression;
    return true;
}

bool handle_id(struct ast_node* node)
{
    if (PRINT) printf("\tparser: found ID as statement beginning!\n");
    struct ast_node* id = ast_create_node();
    string* var_name = NULL;
    EXPECT(parse_id(&var_name));
    id->d.string_data = var_name;
    // pokud se nic nestane, tak je to promenna
    id->type = AST_VAR;

    // leva zavorka za idckem znaci, ze slo o funkci
    if (accept(LPAR)) {
        id->type = AST_CALL;
        EXPECT(token_left_par());
        // nacteme parametry volani
        while (! accept(RPAR)) {
            if ( PRINT ) printf("\tparser: fn call argument parsing\n");
            EXPECT(parse_id(&var_name));
            // vytvorime promennou
            struct ast_node* variable = ast_create_node();
            variable->type = AST_VAR;
            variable->d.string_data = var_name;
            // zalozime do seznamu argumentu
            ast_list_insert(id->d.list, variable);
            if (! accept(RPAR)) {
                EXPECT(token_comma());
            }
        }
        node = id;
    // byla to promenna
    } else if (accept(EQUALS)) {
        node->type = AST_ASSIGN;
        node->left = id;

        expect(EQUALS);

        struct ast_node* expr = ast_create_node();
        EXPECT(parse_expression(expr));

        node->right = expr;
    }

    if (PRINT) printf("\tparser: handled id\n");

    return true;
}

bool parse_var_creation(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing var creation!\n");
    struct ast_node* var = ast_create_node();
    struct ast_node* datatype = ast_create_node();
    enum ast_var_type* var_type = malloc(sizeof(enum ast_var_type));
    EXPECT(parse_datatype(var_type));
    node->type = AST_VAR_CREATION;
    datatype->var_type = *var_type;
    node->left = datatype;

    string* var_name = NULL;
    EXPECT(parse_id(&var_name));
    var->d.string_data = var_name;
    var->type = AST_VAR;

    node->right = var;

    if (PRINT) printf("\tparser: parsed var creation\n");

    // je tam jeste rovnase, tudiz zparsujem prirazeni
    if (accept(EQUALS)) {
        EXPECT(token_equals());
        // trosku na hulvata predelame strukturu - vracet ted musime assign
        struct ast_node* var_creation = ast_create_node();
        var_creation->type = AST_VAR_CREATION;
        var_creation->right = node->right;
        var_creation->left = node->left;

        node->left = var_creation;
        node->type = AST_ASSIGN;

        struct ast_node* expr = ast_create_node();
        EXPECT(parse_expression(expr));

        node->right = expr;
    }

    return true;
}

// TODO: mame vsechny z enumu?
bool token_empty()
{
    if (PRINT) printf("\tparser: empty ?\n");
    return accept(END_OF_FILE);
}

bool parse_id(string** id)
{
    if (PRINT) printf("\tparser: ID\n");
    *id = new_str(d->token->value.string);
    EXPECT(expect(IDENTIFIER));

    return true;
}

bool token_datatype()
{
    if (PRINT) printf("\tparser: accepting datatype\n");
    // tohle je hnusny, ale v navrhu jsem s tim nepocital
    // a zas tak moc me to nestve
    return (accept(KW_INT) || accept(KW_STRING) || accept(KW_DOUBLE) || accept(KW_AUTO));
}

bool parse_datatype(enum ast_var_type* var_type)
{
    if (PRINT) printf("\tparser: parsing datatype\n");
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
    } else if (accept(KW_AUTO)) {
        get_token();
        EXPECT(no_errors());
        *var_type = AST_VAR_AUTO;
        return true;
    } else {
        // failure
        if (PRINT) printf("\tparser: SETTING ERROR VALUE\n");
        d->error = CODE_ERROR_SYNTAX;
        return false;
    }
}

bool token_for()
{
    if (PRINT) printf("\tparser: for\n");
    return expect(KW_FOR);
}

bool token_cout()
{
    if (PRINT) printf("\tparser: cout\n");
    return expect(KW_COUT);
}

bool token_cin()
{
    if (PRINT) printf("\tparser: cin\n");
    return expect(KW_CIN);
}

bool token_extop()
{
    if (PRINT) printf("\tparser: >>\n");
    return expect(EXTOP);
}

bool token_insop()
{
    if (PRINT) printf("\tparser: <<\n");
    return expect(INSOP);
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

bool token_equals()
{
    if (PRINT) printf("\tparser: equals\n");
    return expect(EQUALS);
}
