//
// Created by Peter on 10/25/2015.
//

#include "test.h"
#include <stdio.h>
#include <assert.h>

// Call only your testing funcitons in this function
void StartUnitTests(){
	// Add calls to testing functions here
	TestNothing();

	printf("All tests were successfully executed!\n");
}

void TestNothing(){
	assert(1 != 2);
}