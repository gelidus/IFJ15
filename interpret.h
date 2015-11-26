#ifndef INTERPRET_H
#define INTERPRET_H

#include "ast.h"

#define ASTNode struct ast_node
#define ASTList struct ast_list

void InterpretRun(struct ast_node* func_list);

void InterpretNode(ASTNode *node);

void InterpretList(ASTList *list);

void InterpretStatement(ASTNode* statement);

void InterpretAssign(ASTNode* statement);

void InterpretReturn(ASTNode* ret);

void InterpretIf(ASTNode* ifstatement);

void InterpretFunctionCall(ASTNode* func);

void InterpretExpression(ASTNode* expr);

void InterpretBinaryOperation(ASTNode* op);

#undef ASTNode // cleanup style definition for ast node
#undef ASTList

#endif
