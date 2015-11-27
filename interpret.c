#include <stdio.h>
#include <assert.h>
#include "interpret.h"
#include "errors.h"
#include "symbol_table.h"
#include "gc.h"
#include "string.h"

#define ASTNode struct ast_node // definition of ast node for definition file
#define ASTList struct ast_list

struct symbol_table* scopes;
ASTList* functions;

ASTNode *FindFunction(string *name) {
	ASTList* list = functions;
	if (list->elem == NULL) {
		throw_error(CODE_ERROR_SEMANTIC, "No function was defined");
	}

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
		case AST_CIN:
			InterpretCin(node);
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
	if (get_symbol(scopes, var->right->d.string_data) != NULL) {
		throw_error(CODE_ERROR_SEMANTIC, "Variable redefinition");
	}

	Variable *variable = gc_malloc(sizeof(Variable));
	variable->data_type = var->left->var_type;
	variable->data.numeric_data = 0; // null the data

	set_symbol(scopes, var->right->d.string_data, variable);
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
	// left is id with name and type
	// right is expression assigned
	Variable *result = EvaluateExpression(statement->right);
	Variable* current = get_symbol(scopes, statement->left->d.string_data);
	if (current == NULL) {
		throw_error(CODE_ERROR_SEMANTIC, "[Interpret] Variable assigning failed due to missing variable");
	}
	if (current->data_type != result->data_type) {
		throw_error(CODE_ERROR_SEMANTIC, "[Interpret] Assigning bad value to the variable");
	}

	current->data = result->data;
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
	InterpretList(func->right->d.list);

	scope_end(scopes);
}

void InterpretFor(ASTNode *node) {
	scope_start(scopes);
	// retrieve the fields from the node->d.list and execute

	// block is in the left node
	InterpretList(node->left->d.list);

	scope_end(scopes);
}


enum ast_var_type GetVarTypeFromLiteral(enum ast_literal_type type) {
	switch (type) {
		case AST_LITERAL_INT:
			return AST_VAR_INT;
		case AST_LITERAL_STRING:
			return AST_VAR_STRING;
		case AST_LITERAL_REAL:
			return AST_VAR_DOUBLE;
		case AST_LITERAL_NULL:
			return AST_VAR_NULL;
		case AST_LITERAL_TRUE:
		case AST_LITERAL_FALSE:
			return AST_VAR_BOOL;
	}
}

Variable* EvaluateExpression(ASTNode *expr) {
	assert(expr);
	// unpack if the expression is packed
	if (expr->type == AST_EXPRESSION) {
		expr = expr->left;
	}

	// empty epxression
	if (expr == NULL) {
		return NULL;
	}

	Variable* result = NULL;

	// if the left expression is literal, all we do
	// is copy the literal to the top node as a start
	if (expr->type == AST_LITERAL) {
		// expression is literal. Just get the literal type and return value
		result = gc_malloc(sizeof(Variable));;
		result->data_type = GetVarTypeFromLiteral(expr->literal);
		result->data = expr->d;
	} else if (expr->type == AST_BINARY_OP) {
		// evaluate expressions on both sides
		Variable *left = EvaluateExpression(expr->left);
		Variable *right = EvaluateExpression(expr->right);

		if (left->data_type != right->data_type) {
			throw_error(CODE_ERROR_SEMANTIC, "[Interpret][Expression] Provided values are of different types");
		}

		// expression is binary operation, calculate based on the operator
		switch (expr->d.binary) {
			case AST_BINARY_PLUS:
				result = EvaluateBinaryPlus(left, right);
				break;
			case AST_BINARY_MINUS:
				result = EvaluateBinaryMinus(left, right);
				break;
			case AST_BINARY_TIMES:
				break;
			case AST_BINARY_DIVIDE:
				break;
			case AST_BINARY_LESS:
				break;
			case AST_BINARY_MORE:
				break;
			case AST_BINARY_LESS_EQUALS:
				break;
			case AST_BINARY_MORE_EQUALS:
				break;
		}
	} else if (expr->type == AST_VAR) {
		// the expression is variable, return the variable value
		result = gc_malloc(sizeof(Variable));
		// TODO: lookup value in the table
	}

	return result;
}

Variable *EvaluateBinaryPlus(Variable *left, Variable *right) {
	Variable *result = gc_malloc(sizeof(Variable));

	switch (left->data_type) {
		case AST_VAR_INT:
			result->data.numeric_data = (int)(left->data.numeric_data + right->data.numeric_data);
			result->data_type = AST_VAR_INT;
			break;
		case AST_VAR_DOUBLE:
			result->data.numeric_data = (left->data.numeric_data + right->data.numeric_data);
			result->data_type = AST_VAR_DOUBLE;
			break;
		case AST_VAR_STRING:
			result->data.string_data = cat_str(left->data.string_data, right->data.string_data);
			result->data_type = AST_VAR_STRING;
			break;
		case AST_VAR_BOOL:
			result->data.bool_data = left->data.bool_data + right->data.bool_data;
			result->data_type = AST_VAR_BOOL;
			break;
		case AST_VAR_NULL:
			result->data_type = AST_VAR_NULL;
			break;
	}

	return result;
}

Variable *EvaluateBinaryMinus(Variable *left, Variable *right) {
	return NULL;
}

Variable *EvaluateBinaryMult(Variable *left, Variable *right) {
	return NULL;
}

Variable *EvaluateBinaryDivide(Variable *left, Variable *right) {
	return NULL;
}

// cout node is a list of expressions that should be
// printed to the screen
void InterpretCout(ASTNode *cout) {
	ASTList* list = cout->d.list;
	do {
		// this is the list of expressions
		ASTNode* elem = list->elem;
		Variable* result = EvaluateExpression(elem);
		if (result == NULL) {
			continue; // empty expression
		}

		switch (result->data_type) {
			case AST_VAR_INT:
				printf("%d", (int)result->data.numeric_data);
				break;
			case AST_VAR_DOUBLE:
				printf("%g", result->data.numeric_data);
				break;
			case AST_VAR_NULL:
				printf("NULL");
				break;
			case AST_VAR_STRING:
				printf("%s", result->data.string_data->str);
				break;
			case AST_VAR_BOOL:
				printf(result->data.bool_data ? "true" : "false");
				break;
		}

		list = list->next;
	} while (list != NULL);
}

void InterpretCin(ASTNode *cin) {
	ASTList* list = cin->d.list; // id list

	do {
		ASTNode* elem = list->elem;
		// find the variable that should get the input
		Variable *variable = get_symbol(scopes, elem->d.string_data);
		if (variable == NULL) {
			throw_error(CODE_ERROR_SEMANTIC, "[Interpret] Cannot assign input to non existing variable");
		}

		switch (variable->data_type) {
			case AST_VAR_INT:
				scanf("%d", &((int)variable->data.numeric_data));
				break;
			case AST_VAR_DOUBLE:
				scanf("%g", &((float)variable->data.numeric_data));
				break;
			case AST_VAR_STRING: {
				string *input = new_str("");
				char char_input[10];
				while (gets(char_input) != NULL) {
					for (int i = 0; i < 10, char_input[i] != '\0'; i++) {
						add_char(input, char_input[i]);
					}
				}
				variable->data.string_data = input;
			}
		}

		list = list->next;
	} while(list != NULL);
}
