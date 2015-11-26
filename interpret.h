#ifndef INTERPRET_H
#define INTERPRET_H

#include "ast.h"

#define ASTNode struct ast_node
#define ASTList struct ast_list

/*Helpers and converters*/

/*Interpret functions*/

void InterpretRun(struct ast_node* func_list);

void InterpretNode(ASTNode *node);

void InterpretList(ASTList *list);

void InterpretStatement(ASTNode* statement);

void InterpretAssign(ASTNode* statement);

void InterpretReturn(ASTNode* ret);

void InterpretIf(ASTNode* ifstatement);

void InterpretFunctionCall(ASTNode* func);

void EvaluateExpression(ASTNode* expr);

void InterpretBinaryOperation(ASTNode* op);

void InterpretCout(ASTNode* cout);

#undef ASTNode // cleanup style definition for ast node
#undef ASTList

#endif
