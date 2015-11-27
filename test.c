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

// Call only your testing funcitons in this function
void StartUnitTests(){
	// Add calls to testing functions here
	TestString();

	printf("All tests were successfully executed!\n");
}