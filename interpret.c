#include <stdio.h>
#include <assert.h>
#include "interpret.h"
#include "errors.h"
#include "symbol_table.h"

#define ASTNode struct ast_node // definition of ast node for definition file
#define ASTList struct ast_list

struct symbol_table* scopes;
ASTList* functions;

ASTNode *FindFunction(string *name) {
	ASTList* list = functions;
	do {
		ASTNode* func = list->elem;
		if (equals(func->d.string_data, name)) {
			// interpret definition of main
			return func;
		}

		list = list->next;
	} while(list != NULL);

	return NULL;
}

void InterpretInit(ASTList* fcns) {
	scopes = init_table();
	functions = fcns;
}

void InterpretRun() {
	ASTNode* func = FindFunction(new_str("main"));
	if (func == NULL) {
		throw_error(CODE_ERROR_SEMANTIC, "Main function could not be found");
	}
	InterpretFunctionCall(func);
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
		case AST_FOR:
			InterpretFor(node);
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

	scope_start(scopes);
	// TODO: copy passed arguemnts to our scope

	// list of statements that should be interpreted
	// is in the right leaf of the function
	ASTList* list = func->right->d.list;
	do {
		InterpretNode(list->elem);

		list = list->next;
	} while (list != NULL);

	scope_end(scopes);
}


void InterpretFor(ASTNode *node) {

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
				if(expr->left->type == AST_LITERAL && expr->right->type == AST_LITERAL) {
						if(expr->left->literal == expr->right->literal) {
							result->literal = expr->left->literal;
						}
						else { // if literal is not the same then it has to be real
							result->literal = AST_LITERAL_REAL;
						}
						result->type = AST_LITERAL;
						result->d.numeric_data = expr->left->d.numeric_data + expr->right->d.numeric_data;
						return;
				}
				else if(expr->left->type == AST_EXPRESSION) {
					EvaluateExpression(expr->left);
					EvaluateExpression(result);
				}
				else if(expr->right->type == AST_EXPRESSION) {
					EvaluateExpression(expr->right);
					EvaluateExpression(result);
				}
				break;
			case AST_BINARY_MINUS:
				if(expr->left->type == AST_LITERAL && expr->right->type == AST_LITERAL) {
						if(expr->left->literal == expr->right->literal) {
							result->literal = expr->left->literal;
						}
						else { // if literal is not the same then it has to be real
							result->literal = AST_LITERAL_REAL;
						}
						result->type = AST_LITERAL;
						result->d.numeric_data = expr->left->d.numeric_data - expr->right->d.numeric_data;
						return;
				}
				else if(expr->left->type == AST_EXPRESSION) {
					EvaluateExpression(expr->left);
					EvaluateExpression(result);
				}
				else if(expr->right->type == AST_EXPRESSION) {
					EvaluateExpression(expr->right);
					EvaluateExpression(result);
				}
				break;
			case AST_BINARY_TIMES:
				if(expr->left->type == AST_LITERAL && expr->right->type == AST_LITERAL) {
						if(expr->left->literal == expr->right->literal) {
							result->literal = expr->left->literal;
						}
						else { // if literal is not the same then it has to be real
							result->literal = AST_LITERAL_REAL;
						}
						result->type = AST_LITERAL;
						result->d.numeric_data = expr->left->d.numeric_data * expr->right->d.numeric_data;
						return;
				}
				else if(expr->left->type == AST_EXPRESSION) {
					EvaluateExpression(expr->left);
					EvaluateExpression(result);
				}
				else if(expr->right->type == AST_EXPRESSION) {
					EvaluateExpression(expr->right);
					EvaluateExpression(result);
				}
				break;
			case AST_BINARY_DIVIDE:
				if(expr->left->type == AST_LITERAL && expr->right->type == AST_LITERAL) {
						if(expr->left->literal == expr->right->literal) {
							result->literal = expr->left->literal;
						}
						else { // if literal is not the same then it has to be real
							result->literal = AST_LITERAL_REAL;
						}
						result->type = AST_LITERAL;
						result->d.numeric_data = expr->left->d.numeric_data / expr->right->d.numeric_data;
						return;
				}
				else if(expr->left->type == AST_EXPRESSION) {
					EvaluateExpression(expr->left);
					EvaluateExpression(result);
				}
				else if(expr->right->type == AST_EXPRESSION) {
					EvaluateExpression(expr->right);
					EvaluateExpression(result);
				}
				break;
			case AST_BINARY_LESS:
				if(expr->left->type == AST_LITERAL && expr->right->type == AST_LITERAL) {
						if(expr->left->d.numeric_data < expr->right->d.numeric_data) {
							result->literal = AST_LITERAL_TRUE;
							result->d.numeric_data = 1;
						}
						else {
							result->literal = AST_LITERAL_FALSE;
							result->d.numeric_data = 0;
						}
						result->type = AST_LITERAL;
						return;
				}
				else if(expr->left->type == AST_EXPRESSION) {
					EvaluateExpression(expr->left);
					EvaluateExpression(result);
				}
				else if(expr->right->type == AST_EXPRESSION) {
					EvaluateExpression(expr->right);
					EvaluateExpression(result);
				}
				break;
			case AST_BINARY_MORE:
				if(expr->left->type == AST_LITERAL && expr->right->type == AST_LITERAL) {
						if(expr->left->d.numeric_data > expr->right->d.numeric_data) {
							result->literal = AST_LITERAL_TRUE;
							result->d.numeric_data = 1;
						}
						else {
							result->literal = AST_LITERAL_FALSE;
							result->d.numeric_data = 0;
						}
						result->type = AST_LITERAL;
						return;
				}
				else if(expr->left->type == AST_EXPRESSION) {
					EvaluateExpression(expr->left);
					EvaluateExpression(result);
				}
				else if(expr->right->type == AST_EXPRESSION) {
					EvaluateExpression(expr->right);
					EvaluateExpression(result);
				}
				break;
			case AST_BINARY_EQUALS:
				if(expr->left->type == AST_LITERAL && expr->right->type == AST_LITERAL) {
						if(expr->left->d.numeric_data == expr->right->d.numeric_data) {
							result->literal = AST_LITERAL_TRUE;
							result->d.numeric_data = 1;
						}
						else {
							result->literal = AST_LITERAL_FALSE;
							result->d.numeric_data = 0;
						}
						result->type = AST_LITERAL;
						return;
				}
				else if(expr->left->type == AST_EXPRESSION) {
					EvaluateExpression(expr->left);
					EvaluateExpression(result);
				}
				else if(expr->right->type == AST_EXPRESSION) {
					EvaluateExpression(expr->right);
					EvaluateExpression(result);
				}
				break;
			case AST_BINARY_NOT_EQUALS:
				if(expr->left->type == AST_LITERAL && expr->right->type == AST_LITERAL) {
						if(expr->left->d.numeric_data != expr->right->d.numeric_data) {
							result->literal = AST_LITERAL_TRUE;
							result->d.numeric_data = 1;
						}
						else {
							result->literal = AST_LITERAL_FALSE;
							result->d.numeric_data = 0;
						}
						result->type = AST_LITERAL;
						return;
				}
				else if(expr->left->type == AST_EXPRESSION) {
					EvaluateExpression(expr->left);
					EvaluateExpression(result);
				}
				else if(expr->right->type == AST_EXPRESSION) {
					EvaluateExpression(expr->right);
					EvaluateExpression(result);
				}
				break;
			case AST_BINARY_LESS_EQUALS:
				if(expr->left->type == AST_LITERAL && expr->right->type == AST_LITERAL) {
						if(expr->left->d.numeric_data <= expr->right->d.numeric_data) {
							result->literal = AST_LITERAL_TRUE;
							result->d.numeric_data = 1;
						}
						else {
							result->literal = AST_LITERAL_FALSE;
							result->d.numeric_data = 0;
						}
						result->type = AST_LITERAL;
						return;
				}
				else if(expr->left->type == AST_EXPRESSION) {
					EvaluateExpression(expr->left);
					EvaluateExpression(result);
				}
				else if(expr->right->type == AST_EXPRESSION) {
					EvaluateExpression(expr->right);
					EvaluateExpression(result);
				}
				break;
			case AST_BINARY_MORE_EQUALS:
				if(expr->left->type == AST_LITERAL && expr->right->type == AST_LITERAL) {
						if(expr->left->d.numeric_data >= expr->right->d.numeric_data) {
							result->literal = AST_LITERAL_TRUE;
							result->d.numeric_data = 1;
						}
						else {
							result->literal = AST_LITERAL_FALSE;
							result->d.numeric_data = 0;
						}
						result->type = AST_LITERAL;
						return;
				}
				else if(expr->left->type == AST_EXPRESSION) {
					EvaluateExpression(expr->left);
					EvaluateExpression(result);
				}
				else if(expr->right->type == AST_EXPRESSION) {
					EvaluateExpression(expr->right);
					EvaluateExpression(result);
				}
				break;
		}
	}
}

/*
void standartBinaryOperation(ASTNode * left, ASTNode* right, ast_binary_op_type op_type, ASTNode* result) {
	if(left->type == AST_LITERAL && righ->type == AST_LITERAL) {
			if(left->literal == rigth->literal) {
				result->literal = left->literal;
			}
			else { // if literal is not the same then it has to be real
				result->literal = AST_LITERAL_REAL;
			}
			result->type = AST_LITERAL
			switch (op_type) {
				case AST_BINARY_PLUS:
					result->d.numeric_data = left.d.numeric_data + right.d.numeric_data;
					break;
			}
			return;
	}
	else if(left->type == AST_EXPRESSION) {
		EvaluateExpression(left);
	}
	else if(right->type == AST_EXPRESSION) {
		EvaluateExpression(right);
	}
}
*/
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
