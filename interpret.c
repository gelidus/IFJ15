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
			EvaluateExpression(node->left);
			break;
		case AST_COUT:
			InterpretCout(node);
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
	if (func->d.list == NULL || func->d.list->elem == NULL) {
		return; // function is empty
	}

	// list of statements that should be interpreted
	// is in the right leaf of the function
	ASTList* list = func->right->d.list;
	do {
		InterpretNode(list->elem);

		list = list->next;
	} while (list != NULL);
}

void EvaluateExpression(ASTNode *expr) {
	// empty expression
	if (expr == NULL) {
		return;
	}

	// if the expression is literal, we dont evaluate anything
	if (expr->type == AST_LITERAL) {
		return;
	}
}

void InterpretBinaryOperation(ASTNode *op) {

}

// cout node is a list of expressions that should be
// printed to the screen
void InterpretCout(ASTNode *cout) {
	ASTList* list = cout->d.list;
	do {
		// this is the list of expressions
		ASTNode* elem = list->elem->left;
		EvaluateExpression(elem);

		switch (elem->type) {
			case AST_LITERAL:
				switch (elem->literal) {
					case AST_LITERAL_STRING:
						printf("%s", elem->d.string_data->str);
						break;
					case AST_LITERAL_INT:
						printf("%d", (int)elem->d.numeric_data);
						break;
				}
				break;
			default:
				throw_error(CODE_ERROR_SEMANTIC, "Cout cant interpret given value");
		}

		list = list->next;
	} while (list != NULL);
}
