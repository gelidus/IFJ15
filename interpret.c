#include <stdio.h>
#include "interpret.h"
#include "string.h"
#include "errors.h"

#define ASTNode struct ast_node // definition of ast node for definition file
#define ASTList struct ast_list

void InterpretList(ASTList* tree) {
	// define node for the interation
	ASTList *list = tree;

	// interpret the list
	do {
		// retrieve the node from the list
		ASTNode *node = list->elem;
		switch (node->type) {
			case AST_ASSIGN:
				InterpretAssign(node);
				break;
			case AST_RETURN:
				InterpretReturn(node);
				break;
			case AST_FUNCTION:
				InterpretFunctionCall(node);
				break;
			default:
				throw_error(CODE_ERROR_RUNTIME_OTHER, "Provided ASTNode type not recognized");
		}

		list = list->next;
	} while (list != NULL);
}

void InterpretStatement(ASTNode *statement) {

}

void InterpretAssign(ASTNode *statement) {

}

void InterpretReturn(ASTNode *ret) {

}

void InterpretIf(ASTNode *ifstatement) {

}

void InterpretFunctionCall(ASTNode *func) {

}

void InterpretBinaryOperation(ASTNode *op) {

}
