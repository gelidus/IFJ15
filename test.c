//
// Created by Peter on 10/25/2015.
//

#include "test.h"
#include <stdio.h>
#include <assert.h>
#include "ial.h"
#include "string.h"
#include <string.h>

void TestString() {
	string* str = new_str("hello");
	assert(str != NULL);
	assert(strcmp(str->str, "hello") == 0);
}

void TestSymbolTableInit() {
	SymbolTableInit();
}

void TestSymbolTableScope() {
	string* name = new_str("scope");
	Scope* scope = SymbolTableFindScope(name);
	assert(scope == NULL);

	SymbolTableCreateScope(new_str("scope"));
	scope = SymbolTableFindScope(new_str("scope"));
	assert(scope != NULL);
	assert(equals(scope->name, new_str("scope")));
}

void TestSymbolTableVariable() {
	string* var_name = new_str("var"), *scope_name = new_str("scope");
	Variable* var = SymbolTableFindVariable(scope_name, var_name);
	assert(var == NULL);

	struct ast_node* var_node = ast_create_node();
	var_node->d.string_data = var_name;

	var = SymbolTableAddVariable(scope_name, var_node);
	assert(var != NULL);
	assert(equals(var->var->d.string_data, var_name));

	var = SymbolTableFindVariable(scope_name, var_name);
	assert(var != NULL);
	assert(equals(var->var->d.string_data, var_name));
}

// Call only your testing funcitons in this function
void StartUnitTests(){
	// Add calls to testing functions here
	TestString();

	TestSymbolTableInit();
	TestSymbolTableScope();
	TestSymbolTableVariable();

	printf("All tests were successfully executed!\n");
}