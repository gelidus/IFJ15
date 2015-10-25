#ifndef INTERPRET_H
#define INTERPRET_H

#include "ast.h"

#define ASTNode struct ast_node

void InterpretAST(ASTNode* tree);

void InterpretStatement(ASTNode *statement);

void InterpretVarDeclaration(ASTNode *var);

void InrerpretAssign(ASTNode *statement);

void InterpretReturn(ASTNode *ret);

void InterpretIf(ASTNode *ifstatement);

void InterpretFunctionDeclaration(ASTNode *func);

void InterpretFunctionDefinition(ASTNode *func);

void InterpretFunctionCall(ASTNode *func);

void InterpretBinaryOperation(ASTNode *op);

#undef ASTNode // cleanup style definition for ast node

#endif
