#include "common.h"
#include "parser.h"
#include "gc.h"
#include "interpret.h"

struct data* d;


int check_params(int argc, char *argv[]);

int main(int argc, char *argv[]) {
#ifdef ONLY_TEST
	// launch only tests ...

	return 0;
#else

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
#endif
}

int check_params(int argc, char *argv[])
{
	if (argc < 2) {
		return CODE_ERROR_LEX;
	}

	// vsechno proslo
	return 0;
}
