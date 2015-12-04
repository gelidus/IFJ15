#ifndef INTERPRET_H
#define INTERPRET_H

#include "ast.h"

#define ASTNode struct ast_node
#define ASTList struct ast_list

typedef struct {
    enum ast_var_type data_type;
    union ast_node_data data;
} Variable;

/*Interpret functions*/

// FindFunction will search for the given function
ASTNode* FindFunction(string* name);

void InterpretInit(ASTList* functions);

// InterpretRun will search for the main funciton
// in the given function list node. This function
// will be called afterwards
void InterpretRun();

void InterpretNode(ASTNode *node, Variable* return_val);

void InterpretVarCreation(ASTNode* var);

void InterpretList(ASTList *list, Variable* return_val);

void InterpretAssign(ASTNode* statement);

void InterpretIf(ASTNode* ifstatement, Variable* return_val);

Variable* InterpretFunctionCall(ASTNode* call);

void InterpretFor(ASTNode* node, Variable* return_val);

enum ast_var_type GetVarTypeFromLiteral(enum ast_literal_type type);

Variable* EvaluateExpression(ASTNode* node);

Variable* EvaluateBinaryPlus(Variable* left, Variable* right);

Variable* EvaluateBinaryMinus(Variable* left, Variable* right);

Variable* EvaluateBinaryMult(Variable* left, Variable* right);

Variable* EvaluateBinaryMore(Variable *left, Variable* right);

Variable* EvaluateBinaryDivide(Variable* left, Variable* right);

Variable* EvaluateBinaryLess(Variable *left, Variable *right);

Variable *EvaluateBinaryLessEqual(Variable *left, Variable *right);

Variable *EvaluateBinaryMoreEqual(Variable *left, Variable *right);

Variable *EvaluateBinaryEqual(Variable *left, Variable *right);

Variable *EvaluateBinaryNotEqual(Variable *left, Variable *right);


void InterpretCout(ASTNode* cout);

void InterpretCin(ASTNode* cin);

#undef ASTNode // cleanup style definition for ast node
#undef ASTList

#endif
