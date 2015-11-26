#include <stdio.h>
#include <assert.h>
#include "interpret.h"
#include "string.h"
#include "errors.h"
#include "stack.h"

#define ASTNode struct ast_node // definition of ast node for definition file
#define ASTList struct ast_list

Stack stack;

void CreateScope() {

}

void CreateVariable(ASTNode *node) {

}

void GetVariable(string *name) {

}

void SetVariable(string *name, union ast_node_data node_data) {

}

bool HasVariable(string *name) {
	return false;
}

// InterpretRun will search for the main funciton
// in the given function list node. This function
// will be called afterwards
void InterpretRun(struct ast_node *func_list) {
	ASTList* list = func_list->d.list;
	if (list == NULL) {
		throw_error(CODE_ERROR_INTERNAL, "[Interpret] List of provided functions is null");
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
		case AST_VAR_CREATION:
			InterpretVarCreation(node);
			break;
		case AST_NONE:
			// Empty Statement can happen from trailing semicolons after the expressions.
			// Warn: This is hotfix
			return;
		default:
			throw_error(CODE_ERROR_RUNTIME_OTHER, "[Interpret] Provided ASTNode type not recognized");
	}
}

void InterpretVarCreation(ASTNode *var) {

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

void EvaluateExpression(ASTNode *result) {
	assert(result);
	// empty expression. result should never
	// be null, only it's left side
	if (result->left == NULL) {
		return;
	}

	// this is needed, as the expressions are packed
	ASTNode* expr = result->left;

	// if the left expression is literal, all we do
	// is copy the literal to the top node as a result
	if (expr->type == AST_LITERAL) {
		result->type = expr->type;
		result->literal = expr->literal;
		result->d = expr->d;
		return;
	}

	if (expr->type == AST_EXPRESSION) {
		EvaluateExpression(expr);
	}

	// TODO
	if (expr->type == AST_BINARY_OP) {
		switch (expr->d.binary) {
			case AST_BINARY_PLUS:
				break;
		}
	}
}

// cout node is a list of expressions that should be
// printed to the screen
void InterpretCout(ASTNode *cout) {
	ASTList* list = cout->d.list;
	do {
		// this is the list of expressions
		ASTNode* elem = list->elem;
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
			case AST_EXPRESSION: // TODO: just for testing, remove this when everything should be ok
				break;
			default:
				throw_error(CODE_ERROR_SEMANTIC, "Cout cant interpret given value");
		}

		list = list->next;
	} while (list != NULL);
}
