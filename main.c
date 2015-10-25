#include "common.h"
#include "parser.h"
#include "gc.h"
#include "interpret.h"

struct data* d;


int check_params(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	// nebudeme do mainu zanaset kousky logiky, testy na existenci zdrojoveho souboru apod. testujeme tady
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

int check_params(int argc, char *argv[])
{
	if (argc < 2) {
		return CODE_ERROR_LEX;
	}

	// vsechno proslo
	return 0;
}
