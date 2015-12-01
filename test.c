//
// Created by Peter on 10/25/2015.
//

#include "test.h"
#include <stdio.h>
#include <assert.h>
#include "ial.h"
#include "string.h"
#include <string.h>
#include "symbol_table.h"

void TestString() {
	string* str = new_str("hello");
	assert(str != NULL);
	assert(strcmp(str->str, "hello") == 0);
}

void testHashTable() {
	struct symbol_table * t = init_table();
	scope_start(t);
	set_symbol(t, new_str("a"), new_str("value_a"));
	set_symbol(t, new_str("b"), new_str("value_b"));
	string * val_a = get_symbol(t, new_str("a"));
	string * val_b = get_symbol(t, new_str("b"));
	assert(equals(val_a, new_str("value_a")));
	scope_end(t);
}

// Call only your testing funcitons in this function
void StartUnitTests(){
	// Add calls to testing functions here
	TestString();
	testHashTable();
	printf("All tests were successfully executed!\n");
}
