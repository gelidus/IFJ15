#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "input.h"
#include "ial.h"
#include "finite.h"

int main(int argc, char** argv) {
   //tSTable STable;
   //htInit(&STable); // inicializace tabulky symbolu

	int result = NO_ERROR;
	
	set_input(argv[1]);

	const static struct {
	  char* lexem;	  
	} lexems[] = {
	  {"NO TYPE"},
	  {"identifier"},
	  {"kw auto"}, {"kw cin"}, {"kw cout"}, {"kw double"}, {"kw else"}, {"kw for"}, {"kw if"}, {"kw int"}, {"kw return"}, {"kw string"},
	  {"integer"}, {"double"}, {"string"},
	  {"+"}, {"-"}, {"*"}, {"/"}, {"="},
	  {"<"}, {">"}, {"<="}, {">="}, {"=="}, {"!="}, {"<<"}, {">>"},
	  {"("}, {")"}, {"{"}, {"}"}, 
	  {";"}, {","},
	  {"EOF"}
	};
	
	printf("%d\n", find("hello world hello", "ell"));
	printf("%d\n", find("hello world hello", "z"));
	printf("%d\n", find("hello world hello", ""));
	printf("%s\n", sort("hello world"));

	if(argv[1] == NULL) {
		return -1;
	}
	struct lexeme lexem_data;
	enum lex_type type = NO_TYPE;
	do {
		lexem_data = read_lexeme();		
		type = lexem_data.type;
		if(type == INTEGER) {
			fprintf(stdout, "%s %d\n", lexems[type].lexem, lexem_data.value.integer);
		} else if(type == DOUBLE) {
			fprintf(stdout, "%s %f\n", lexems[type].lexem, lexem_data.value.real);
		} else if(type == STRING || type == IDENTIFIER) {
			fprintf(stdout, "%s %s\n", lexems[type].lexem, lexem_data.value.string);			
		} else {
			fprintf(stdout, "%s\n", lexems[type].lexem);
		}
	} while(type != END_OF_FILE);	

	if(result != NO_ERROR) {
		//htClearAll(&STable);              
		fprintf(stderr, "Error number %d\n", result);
		return result;
	}
   
   //htClearAll(&STable);

   return NO_ERROR;
}
