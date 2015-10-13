#include <stdio.h>

struct data d;

int main(int argc, char *argv[]) {
	// nebudeme do mainu zanaset kousky logiky, testy na existenci zdrojoveho souboru apod. testujeme tady
	if (int result = check_params(argc, argv)) {
		return result;
	}

	// naalokujeme sdilenou datovou strukturu
	d = malloc(sizeof(struct data));

	// parse
	parser_prepare(*d);
	*d = parser_run();

	const char *file = argv[1]; // save this to the scanner instead
}

int check_params(int argc, char *argv[])
{
	if (argc < 2) {
		return CODE_ERROR_LEX;
	}

	// vsechno proslo
	return 0;
}
