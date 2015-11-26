#ifndef INTERPRET_H
#define INTERPRET_H

#include "ast.h"

#define ASTNode struct ast_node
#define ASTList struct ast_list

union ast_node_data
{
    enum ast_binary_op_type binary; // typ binarni operace
    string* string_data; // nazev fce, nebo promenne, text
    double numeric_data;
    struct ast_node* condition; //pro podminku u if
    struct ast_list* list; // pro uchovavani agumentu, statement body, function body, if body, else body..
};

typedef struct {
    enum ast_var_type data_type;
    union ast_node_data data;
} Variable;

/*Helpers*/

void CreateScope();

void CreateVariable(ASTNode* node);

void GetVariable(string* name);

void SetVariable(string* name, union ast_node_data);

bool HasVariable(string* name);

/*Interpret functions*/

void InterpretRun(struct ast_node* func_list);

void InterpretNode(ASTNode *node);

void InterpretVarCreation(ASTNode* var);

void InterpretList(ASTList *list);

void InterpretStatement(ASTNode* statement);

void InterpretAssign(ASTNode* statement);

void InterpretReturn(ASTNode* ret);

void InterpretIf(ASTNode* ifstatement);

void InterpretFunctionCall(ASTNode* func);

void EvaluateExpression(ASTNode* expr);

void InterpretCout(ASTNode* cout);

#undef ASTNode // cleanup style definition for ast node
#undef ASTList

#endif
