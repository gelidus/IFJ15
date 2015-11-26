#include <stdio.h>
#include "interpret.h"
#include "string.h"
#include "errors.h"

#define ASTNode struct ast_node // definition of ast node for definition file
#define ASTList struct ast_list


// InterpretRun will search for the main funciton
// in the given function list node. This function
// will be called afterwards
void InterpretRun(struct ast_node *func_list) {
	ASTList* list = func_list->d.list;
	if (list == NULL) {
		// what?
		return;
	}

	do {
		ASTNode* func = list->elem;
		if (equals(func->d.string_data, new_str("main"))) {
			// interpret definition of main
			InterpretFunctionCall(func);
			return;
		}

		list = list->next;
	} while(list != NULL);

	throw_error(CODE_ERROR_SEMANTIC, "Main function could not be found");
}

void InterpretNode(ASTNode *node) {
	switch(node->type) {
		// retrieve the node from the list
		case AST_ASSIGN:
			InterpretAssign(node);
			break;
		case AST_RETURN:
			InterpretReturn(node);
			break;
		case AST_FUNCTION:
			InterpretFunctionCall(node);
			break;
		case AST_EXPRESSION:
			// first expression node is in the left leaf of the expression (see expression parser)
			InterpretExpression(node->left);
			break;
		default:
			throw_error(CODE_ERROR_RUNTIME_OTHER, "Provided ASTNode type not recognized");
	}
}

void InterpretList(ASTList* list) {
	// no interpretation for empty list needed
	if (list->elem == NULL) {
		return;
	}

	do {
		// interpret node on current leaf
		InterpretNode(list->elem);
		// change leaf to next
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

void InterpretExpression(ASTNode *expr) {
	// empty expression
	if (expr == NULL) {
		return;
	}
}

void InterpretBinaryOperation(ASTNode *op) {

}
