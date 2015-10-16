// ENUMY PRO abstraktni syntakticky strom

// mozna se nepouzijou, jeste nevim !


enum ast_node_type
{
    AST_STATEMENT, // v sobe obsahuje cokoliv
    AST_ASSIGN, // prirazeni do promenne
    AST_WHILE, // cyklus while
    AST_RETURN, // return
    AST_LITERAL, // string, cisla, false, true, null
    AST_IF, // ze by if?

    AST_FUNCTION, //vytvareni funkce
    AST_CALL, //volani fce
    AST_VAR, //promenna
    AST_BINARY_OP //binarni operace +-*/
};

enum ast_var_type
{
    AST_VAR_INT,
    AST_VAR_DOUBLE,
    AST_VAR_STRING,
    AST_VAR_BOOLEAN,
    AST_VAR_NULL,  // pouze hodnota null
};

enum ast_literal_type
{
    AST_LITERAL_FALSE,
    AST_LITERAL_TRUE,
    AST_LITERAL_NULL,
    AST_LITERAL_NUMERIC,
    AST_LITERAL_STRING
};

enum ast_binary_op_type
{
    AST_BINARY_MINUS, //0
    AST_BINARY_PLUS, //1
    AST_BINARY_TIMES, //2
    AST_BINARY_DIVIDE, //3
    AST_BINARY_LESS, //4
    AST_BINARY_MORE, //5
    AST_BINARY_EQUALS, //6
    AST_BINARY_NOT_EQUALS, //7
    AST_BINARY_LESS_EQUALS, //8
    AST_BINARY_MORE_EQUALS, //9
    AST_BINARY_STRICT_EQUALS, //10
    AST_BINARY_STRICT_NOT_EQUALS, //11
	AST_BINARY_STRING_CONCATENATION //12
};
