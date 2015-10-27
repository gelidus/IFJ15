#include "common.h"
#include "parser.h"
#include "gc.h"
#include "interpret.h"

struct data* d;


int check_params(int argc, char *argv[]);

#ifdef UNIT_TEST
#include "test.h"
// This is only testing main which should call testing
// module. No direct interpretation will be done here.
// Warning: undef ONLY_TEST if you want to correctly test
// and build interpret
int main() {
	StartUnitTests();
}
#else
// Main entry to the interpret. No tests will be launched, only
// pure interpret will be compiled
int main(int argc, char *argv[]) {
	int result;
	if ((result = check_params(argc, argv))) {
		return result;
	}

	// naalokujeme sdilenou datovou strukturu
	d = (struct data* )gc_malloc(sizeof(struct data));

	// parse
	parser_prepare(d);
	d = parser_run();

	return 0;
}
#endif

int check_params(int argc, char *argv[])
{
	if (argc < 2) {
		return CODE_ERROR_LEX;
	}

	// vsechno proslo
	return 0;
}
