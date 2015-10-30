#ifndef INTERPRET_H
#define INTERPRET_H

#include "ast.h"

#define ASTNode struct ast_node
#define ASTList struct ast_list

void InterpretList(ASTList* tree);

void InterpretStatement(ASTNode *statement);

void InterpretAssign(ASTNode *statement);

void InterpretReturn(ASTNode *ret);

void InterpretIf(ASTNode *ifstatement);

void InterpretFunctionCall(ASTNode *func);

void InterpretBinaryOperation(ASTNode *op);

#undef ASTNode // cleanup style definition for ast node
#undef ASTList

#endif
