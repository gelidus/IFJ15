#ifndef INTERPRET_H
#define INTERPRET_H

#include "ast.h"

#define ASTNode struct ast_node
#define ASTList struct ast_list

union variable_data
{
    enum ast_binary_op_type binary; // typ binarni operace
    string* string_data; // nazev fce, nebo promenne, text
    double numeric_data;
    struct ast_node* condition; //pro podminku u if
    struct ast_list* list; // pro uchovavani agumentu, statement body, function body, if body, else body..
};

typedef struct {
    enum ast_var_type data_type;
    union variable_data data;
} Variable;

/*Interpret functions*/

// FindFunction will search for the given function
ASTNode* FindFunction(string* name);

void InterpretInit(ASTList* functions);

// InterpretRun will search for the main funciton
// in the given function list node. This function
// will be called afterwards
void InterpretRun();

void InterpretNode(ASTNode *node);

void InterpretVarCreation(ASTNode* var);

void InterpretList(ASTList *list);

void InterpretStatement(ASTNode* statement);

void InterpretAssign(ASTNode* statement);

void InterpretReturn(ASTNode* ret);

void InterpretIf(ASTNode* ifstatement);

void InterpretFunctionCall(ASTNode* func);

void InterpretFor(ASTNode* node);

void EvaluateExpression(ASTNode* expr);

void InterpretCout(ASTNode* cout);

#undef ASTNode // cleanup style definition for ast node
#undef ASTList

#endif
