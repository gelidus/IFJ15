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

bool parse_id(string* name);
bool parse_datatype(enum ast_var_type* var_type);
bool parse_var_creation(struct ast_node* node);
bool parse_function_definition(struct ast_node* node);
bool parse_function_arguments(struct ast_node* node);
bool parse_cout(struct ast_node* node);
bool parse_cin(struct ast_node* cin);
bool parse_program_body();
bool parse_statement(struct ast_node* instr, bool in_root);
bool parse_expression(struct ast_node* node);
bool parse_if(struct ast_node* node);
bool parse_return(struct ast_node* node);
bool parse_for(struct ast_node* node);
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
    int result = expect(parse_program_body());
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

bool parse_statement(struct ast_node* node, bool in_root)
{
    // doplnit, az bude jasne, co jak vypada
    if (PRINT) printf("\tparser: handling statement\n");
    if (token_datatype()) {
        EXPECT(parse_var_creation(node));
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
    // tyhle jsou uz fallback!
    } else if (token_empty()) {
        // prazdny statement je validni
        return true;
    } else if (accept(SEMICOLON)) {
        EXPECT(token_semicolon());
    } else {
        // vsechno ostatni je pro me vyraz
        EXPECT(parse_expression(node));
    }

    EXPECT(no_errors());

    return true;
}

bool parse_program_block(struct ast_node* node, bool in_root)
{
    if (PRINT) printf("\tparser: handling program block\n");
    // priprav seznam instrukci bloku
    node->d.list = ast_create_list();

    struct ast_node* new_node;
    while(true) {
        // posledni token ale nejsme v bloku?
        if (!in_root && token_empty()) {
            if (PRINT) printf("\tparser: SETTING ERROR VALUE\n");
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

        // v rootu se obevi jen deklarace funkci
        if (in_root) {
            EXPECT(parse_function_definition(new_node));
        // vsude jinde to budou statementy
        } else {
            EXPECT(parse_statement(new_node, in_root))
        }

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

bool parse_for(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing for\n");

    struct ast_node* first_field = ast_create_node();
    struct ast_node* second_field = ast_create_node();
    struct ast_node* third_field = ast_create_node();
    struct ast_node* block = ast_create_node();

    EXPECT(token_for());
    // podminky
    EXPECT(token_left_par())
    EXPECT(parse_expression(first_field));
    EXPECT(token_semicolon());
    EXPECT(parse_expression(second_field))
    EXPECT(token_semicolon());
    EXPECT(parse_expression(third_field));

    EXPECT(token_left_brace())
    EXPECT(parse_program_block(block, false));
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

bool parse_cout(struct ast_node* node)
{
    if (PRINT) printf("\tparser: parsing cout\n");

    node->d.list = ast_create_list();

    EXPECT(token_cout());

    while (! accept(SEMICOLON)) {
        EXPECT(token_insop());
        struct ast_node* expression = ast_create_node();
        EXPECT(parse_expression(expression));

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
        EXPECT(parse_id(var));
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

    enum ast_var_type* var_type = malloc(sizeof(enum ast_var_type));
    EXPECT(parse_datatype(var_type));

    string* function_name = NULL;
    EXPECT(parse_id(function_name));

    EXPECT(parse_function_arguments(arguments));

    EXPECT(token_left_brace());
    EXPECT(parse_program_block(body, false));
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
    EXPECT(token_left_par());
    string* var_name = NULL;
    // dokud argumenty neskonci
    while (! accept(RPAR)) {
        if ( PRINT ) printf("\tparser: function argument parsing\n");
        // nejdriv datatyp, potom promenna
        enum ast_var_type* var_type = malloc(sizeof(enum ast_var_type));
        EXPECT(parse_datatype(var_type));
        EXPECT(parse_id(var_name));
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
    /////   +    -    *    /    <    >   <=   >=   <>    =    (    )   ID    $
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

int GetCorrectTokenValue(enum lex_type type) {
    switch(type) {
        case PLUS: return 0;
        case MINUS: return 1;
        case MULT: return 2;
        case DIVIDE: return 3;
        case LT: return 4;
        case GT: return 5;
        case LTE: return 6;
        case GTE: return 7;
        case NEQ: return 8;
        // = ... assignment is not handled by us
        case LPAR: return 10;
        case RPAR: return 11;

        // IDS
        case INTEGER:
        case DOUBLE:
        case STRING:
        case IDENTIFIER:
            return 12;

        case NO_TYPE:
            return 13;

        default:
            // return 12, throw error or whatever is wrong
            return 12;
    }
}

int GetExpressionStackType(Stack *stack) {

    Element *el = StackTop(stack);

    while(el != NULL) {
        struct ast_node* node = el->value;
        if (node->type != AST_EXPRESSION) {
            // return the type of the top stack token
        }

        el = el->next;
    }

    return 13; // no type
}

char GetPrecendence(enum lex_type first, enum lex_type second) {
    return PrecendenceTable[GetCorrectTokenValue(first)][GetCorrectTokenValue(second)];
}

bool parse_expression(struct ast_node* node) {

    if ( PRINT ) printf("\tparser: parsing expression\n");

    Stack stack;
    StackInit(&stack);

    enum ast_node_type stackType = NO_TYPE;
    struct ast_node* source1 = NULL, *source2 = NULL, *result = NULL;

    get_token();

    do { // until $ on all stacks
        // ak nepatri lexem do expression lexemov, potom priradit typ unknown
        if (false) {
        }

        if (StackTop(&stack) == NULL) {
            stackType = AST_NONE;
        } else {
            stackType = ((struct ast_node*)StackTop(&stack))->type;
        }

        char precendenceCharacter = GetPrecendence(stackType, d->token->type);

        switch(precendenceCharacter) {
            case '=':
            case '<': {
                //TODO: push data about expression to stack
                StackPush(&stack, NULL);

                get_token();
                break;
            }
            case '>': {
                struct ast_node* current = StackPop(&stack)->value;

                if (current != NULL && current->type != RPAR) {
                    //// E -> E1 op E2 ////

                    // get second source
                    source2 = current;

                    // get operation
                    result = StackPop(&stack)->value;

                    // get first source
                    source1 = StackPop(&stack)->value;

                    // add leafs to the result

                } else if (current != NULL) {
                    //// E -> (E) ////
                    current = StackPop(&stack)->value;

                    result = current;

                    current = StackPop(&stack)->value;
                    if (current == NULL || current->type != RPAR) {
                        throw_error(CODE_ERROR_SYNTAX, "");
                    }
                } else {
                    throw_error(CODE_ERROR_SYNTAX, "");
                }

                //if(!StackPush(&stack, NULL, EXPRESSION, &destination, function, global)) {
                //    throw_error(CODE_ERROR_SYNTAX);
                //}
                break;
            }
            case '$':
                if(StackTop(&stack) == NULL) {
                    throw_error(CODE_ERROR_SYNTAX, "");
                }

            //ExpressionData *stackTop = popExpressionData(&stack);
            //if(stackTop->tokenType != EXPRESSION) {
            //    return CODE_ERROR_SYNTAX;
            //}

            //destination = stackTop->symbol;
            break;


            default:
                throw_error(CODE_ERROR_SYNTAX, "");
            break;
        }

        //if(current->type != TOKEN_TYPE_NONE) {
            //TODO: FIX IT!! free(currentToken)
        //}
    } while(!StackEmpty(&stack));

    //if(result) {
    //    *result = destination;
    //}

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
    EXPECT(parse_program_block(if_body, false));
    EXPECT(token_right_brace());

    // else blok
    EXPECT(token_else());
    EXPECT(token_left_brace())
    EXPECT(parse_program_block(else_body, false));
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
    EXPECT(parse_id(var_name));
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
            EXPECT(parse_id(var_name));
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

        struct ast_node* expr = ast_create_node();
        EXPECT(parse_expression(expr));

        node->right = expr;
    }
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
    EXPECT(parse_id(var_name));
    var->d.string_data = var_name;
    var->type = AST_VAR;

    node->right = var;

    if (PRINT) printf("\tparser: parsed var creation\n");

    // je tam jeste rovnase, tudiz zparsujem prirazeni
    if (accept(EQUALS)) {
        // trosku na hulvata predelame strukturu - vracet ted musime assign
        struct ast_node* var_creation = ast_create_node();
        var_creation->type = AST_VAR_CREATION;
        var_creation->right = var;
        var_creation->left = node;

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

bool parse_id(string* id)
{
    if (PRINT) printf("\tparser: ID\n");
    EXPECT(expect(IDENTIFIER));

    id = new_str(d->token->value.string);

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
