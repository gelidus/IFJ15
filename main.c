#include "common.h"
#include "parser.h"
#include "gc.h"
#include "interpret.h"
#include "input.h"

struct data* d;


int check_params(int argc, char *argv[]);
void make_data_structure();

#ifdef UNIT_TEST
#include "test.h"
// This is only testing main which should call testing
// module. No direct interpretation will be done here.
// Warning: undef ONLY_TEST if you want to correctly test
// and build interpret
int main() {
	StartUnitTests();

	return 0;
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
	make_data_structure();

	// otevrem soubor
	set_input(argv[1]);

#ifdef LEX_TEST
    do {
		get_token();
    } while (d->token->type != EOF);
#else
	// parse
	parser_prepare(d);
	d = parser_run();

	if (d->error != CODE_OK) {
		return 2;
	}

	InterpretInit(d->tree->d.list);
	// interpret the list
	InterpretRun();
#endif

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

// alokuje a zabezpeci sdilenou datovou strukturu
void make_data_structure()
{
	d = (struct data* )gc_malloc(sizeof(struct data));
	d->error = CODE_OK;
	d->token = NULL;
	d->functions = NULL;
	d->vars = NULL;
	d->tree = NULL;
}
