#include "scanner.h"

#define PRINT 0

const static struct {
  char* id;
  enum lex_type type;
} keywords_table[] = {
  {"auto", KW_AUTO}, {"cin", KW_CIN}, {"cout", KW_COUT}, {"double", KW_DOUBLE}, {"else", KW_ELSE},
  {"for", KW_FOR}, {"if", KW_IF}, {"int", KW_INT}, {"return", KW_RETURN}, {"string", KW_STRING},
  {NULL, NO_TYPE}
};

unsigned char tmp_esc_val[2];
static int input_char; //prave nacteny znak
static enum input_type input_char_type; //prave nacteny typ znaku

static char temp[CHUNK];
//static char* safeStr = "74|75|99|111|112|121|114|105|103|104|116";
static int temp_length;

static void save(struct lexeme *, char); //trvale ulozeni retezce do lexemu
static void save_temp(char); //docasne ulozeni stringu

static int length; //delka retezce
static int chunks; //zvetsovani bufferu

static char *e_strtod; //osetreni parametru pro prevod cisel double
static unsigned char tmpx = 0;


enum lex_type check_keyword(char* candidate) {
  int i = -1;

  while(keywords_table[++i].id) //pruchod celou tabulkou symbolu
    if(!strcmp((const char *)candidate, (const char *)keywords_table[i].id)) //porovnani identifikatoru a kandidatnich klicu
      return keywords_table[i].type; //jedna se o kw

  return IDENTIFIER; //jedna se o identif.
}

void read_input() {
  input_char = get_char(); //nacteni znaku ze vstupu
  //prirazeni typu znaku
  if(tolower(input_char) >= 'a' && tolower(input_char) <= 'z')
    input_char_type = LETTER;
  else if(input_char >= '0' && input_char <= '9')
    input_char_type = DIGIT;
  else if(input_char == EOF)
    input_char_type = END;
  else if(input_char <= 32)
    input_char_type = WHITE_SPACE;
  else if(input_char == '_')
    input_char_type = UNDERSCORE;
  else
    input_char_type = OTHERS;
}

void return_input() {
	return_char(input_char); //vraceni znaku na vstup
}

//hlavni funkce
struct lexeme read_lexeme(void) {
	struct lexeme tmpData;
	tmpData.type = NO_TYPE;

	read_input();

q0: //default state
	if(PRINT) printf("q0 default\n");
	switch(input_char_type) {
		case UNDERSCORE:
		case LETTER:
			temp_length = 0;
			save_temp(input_char);
			read_input();
			goto q1; //identifier
		case DIGIT:
			tmpData.type = INTEGER;
			temp_length = 0;
			save_temp(input_char);
			read_input();
			goto q4; //integer or double
			break;
		case WHITE_SPACE:
			read_input();
			goto q0; //default state
		/*
		case UNDERSCORE:
			temp_length = 0;
			save_temp(input_char);
			read_input();
			goto q1; //underscored identifier
		*/
		case END:
			tmpData.type = END_OF_FILE;
			return tmpData;
		case OTHERS:
			switch(input_char) {
				case '+':
					tmpData.type = PLUS;
					return tmpData;
				case '-':
					tmpData.type = MINUS;
					return tmpData;
				case '*':
					tmpData.type = MULT;
					return tmpData;
				case '/':
					read_input();
					goto q3; //comments or divide
				case '=':
					read_input();
					if(input_char == '=') {
						tmpData.type = EQ;
						return tmpData;
					}
					return_input();
					tmpData.type = EQUALS;
					return tmpData;
				case '<':
					read_input();
					if(input_char == '=') {
						tmpData.type = LTE;
						return tmpData;
					} else if(input_char == '<') {
						tmpData.type = INSOP;
						return tmpData;
					}
					return_input();
					tmpData.type = LT;
					return tmpData;
				case '>':
					read_input();
					if(input_char == '=') {
						tmpData.type = GTE;
						return tmpData;
					} else if(input_char == '>') {
						tmpData.type = EXTOP;
						return tmpData;
					}
					return_input();
					tmpData.type = GT;
					return tmpData;
				case '!':
					read_input();
					if(input_char == '=') {
						tmpData.type = NEQ;
						return tmpData;
					}
					throw_error(CODE_ERROR_LEX, "invalid input");
					break;
				case ';':
					tmpData.type = SEMICOLON;
					return tmpData;
				case '(':
					tmpData.type = LPAR;
					return tmpData;
				case ')':
					tmpData.type = RPAR;
					return tmpData;
				case '{':
					tmpData.type = LBR;
					return tmpData;
				case '}':
					tmpData.type = RBR;
					return tmpData;
				case ',':
					tmpData.type = COLON;
					return tmpData;
				case '"':
					tmpData.type = STRING;
					temp_length = 0;
					chunks = 0;
					length = 0;
					read_input();
					goto q8; //string
				default:
					throw_error(CODE_ERROR_LEX, "invalid input");
					break;
			}
			break;
		default:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;
	}

q1: //identifier OK
	if(PRINT) printf("q1 identifier\n");
	switch(input_char_type) {
		case LETTER:
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q1; //identifier
		case UNDERSCORE:
			save_temp(input_char);
			read_input();
			goto q1; //underscored identifier
		default:
			return_input();
			save_temp(0);
			if((tmpData.type = check_keyword(temp)) == IDENTIFIER) {
				if((tmpData.value.string = (char *)malloc(temp_length)) == NULL)
					throw_error(CODE_ERROR_INTERNAL, "malloc error");
				memcpy((void *)tmpData.value.string, (void *)temp, temp_length);
			}
			return tmpData;
	}
/*
q2: //underscored identifier OK
	if(PRINT) printf("q2 underscored identifier\n");
	switch(input_char_type) {
		case LETTER:
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q1; //identifier
		case UNDERSCORE:
			save_temp(input_char);
			read_input();
			goto q2; //underscored identifier
		default:
			return_input();
			return tmpData;
	}
*/
q3: //comments or divide OK
	if(PRINT) printf("q3 comments or divide\n");
	switch(input_char_type) {
		case LETTER:
		case DIGIT:
		case WHITE_SPACE:
			return_input();
			tmpData.type = DIVIDE;
			return tmpData;
		default:
			switch(input_char) {
				case '/':
					//q11
					do {
						read_input();
					} while(input_char != '\n' && input_char_type != END);
					if(input_char_type == END) {
						tmpData.type = END_OF_FILE;
						return tmpData;
					}
					read_input();
					goto q0; //default state
				case '*':
					//q12
					do {
						do {
							read_input();
						} while(input_char != '*' && input_char_type != END);
						if(input_char_type == END) {
							throw_error(CODE_ERROR_LEX, "invalid input");
							break;
						}
						read_input();
						if(input_char == '/') {
							read_input();
							goto q0; //default state
						} else if(input_char == '*') {
							return_input();
						} else if(input_char_type == END) {
							throw_error(CODE_ERROR_LEX, "invalid input");
							break;
						}
					} while(1);
				default:
					return_input();
					tmpData.type = DIVIDE;
					return tmpData;
			}
			break;
	}

q4: //integer or double OK
	if(PRINT) printf("q4 integer or double\n");
	switch(input_char_type) {
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q4; //integer or double
		/*
		case WHITE_SPACE:
			save_temp(0);
			tmpData.value.integer = atoi((const char *)temp);
			return tmpData;
		*/
		case LETTER:
			if(input_char == 'e' || input_char == 'E') {
				save_temp(input_char);
				read_input();
				tmpData.type = DOUBLE;
				goto q5; //double e
			} else {
				throw_error(CODE_ERROR_LEX, "invalid input");
				break;
			}
		case END:
			/*
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;
			*/
			return_input();
			save_temp(0);
			tmpData.value.integer = atoi((const char *)temp);
			return tmpData;			
		case UNDERSCORE:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;				
		case OTHERS:			
			if(input_char == '.') {
				save_temp(input_char);
				read_input();
				tmpData.type = DOUBLE;
				goto q13; //double
			}
			return_input();
			save_temp(0);
			tmpData.value.integer = atoi((const char *)temp);
			return tmpData;		
		default:
			return_input();
			save_temp(0);
			tmpData.value.integer = atoi((const char *)temp);
			return tmpData;
	}

q5: //double e OK
	if(PRINT) printf("q5 double e\n");
	switch(input_char_type) {
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q7; //double e
		case OTHERS:
			if(input_char == '+' || input_char == '-') {  
				save_temp(input_char);
				read_input();
				goto q14; //double e +-
			}/* else {
				return_input();
				save_temp(0);
				tmpData.value.real = strtod((const char *)temp, &e_strtod);
				return tmpData;
			}*/	
		default:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;
	}

q13: //potential double
	if(PRINT) printf("q13 potential double\n");
	switch(input_char_type) {
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q6; //double
		default:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;				
	} 
	
q14: //potential double e +-
	if(PRINT) printf("q14 potential double e +-\n");
	switch(input_char_type) {
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q7; //double e +-
		default:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;				
	} 	

q6: //double OK
	if(PRINT) printf("q6 double\n");
	switch(input_char_type) {
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q6; //double
		case LETTER:
			if(input_char == 'e' || input_char == 'E') {
				save_temp(input_char);
				read_input();
				goto q5; //double e
			} else {
				throw_error(CODE_ERROR_LEX, "invalid input");
				break;
			}
		/*
		case WHITE_SPACE:
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
		case END:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;
		*/ 
		default:
			return_input();
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
	}

q7: //double e +- OK
	if(PRINT) printf("q7 double e +-\n");
	switch(input_char_type) {
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q7; //double e +-
		/*
		case WHITE_SPACE:
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
		case END:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;
		*/
		default:
			return_input();
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
	}

q8: //string OK
	if(PRINT) printf("q8 string\n");
	switch(input_char_type) {
		case OTHERS:			
			if(input_char == '\\') {
				//save(&tmpData, input_char);
				read_input();
				goto q9;	//escaped string
			} else if(input_char == '"') {
				//q10
				//save(&tmpData, 0);
				return tmpData;
			}
			save(&tmpData, input_char);
			read_input();
			goto q8; //string
		case WHITE_SPACE:			 
			if(input_char == 32) {
				save(&tmpData, input_char);
				read_input();
				goto q8; //string	
			}
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;
		case END:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;
		default:			
			save(&tmpData, input_char);
			read_input();
			goto q8;  //string
	}

q9: //escaped string OK
	if(PRINT) printf("q9 escaped string\n");
	switch(input_char_type) {
		case END:
		if(PRINT) printf("vypis 1\n");
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;			
		default:
		if(PRINT) printf("vypis 2\n");
		
		//  n -> \n   t -> \t   /* LOMITKA DOPLNENA */
		
			if(input_char == 'n' || input_char == 't' || input_char == '\\' || input_char == '"') 
			{
												if(PRINT) printf("vypis 3\n");
			//	save(&tmpData, '\\');   NENMA TU BYT
 				if(input_char == 'n')
 				save(&tmpData, '\n');	
 				else if (input_char == 't')
 				save(&tmpData, '\t');
 				else
				save(&tmpData, input_char);
												if(PRINT) printf("vypis 5\n");
				read_input();
												if(PRINT) printf("vypis 6\n");
				goto q8;  //string
												if(PRINT) printf("vypis 7\n");
			} else if(input_char == 'x') 
												if(PRINT) printf("vypis 8\n");
			{
				read_input();
												if(PRINT) printf("vypis 9\n");
				goto q16;
			}
			throw_error(CODE_ERROR_LEX, "invalid input");
												if(PRINT) printf("\n");
			break;			
	}
	
	
	
q16: //escaped string x

tmpx = 0;
	switch(input_char_type) {
		case LETTER:
									if(PRINT) printf("vypis 10\n");
			if(input_char >= 'A' && input_char <= 'F') {
								tmpx += (input_char - 'A'+10)*16;
				
												break;
									if(PRINT) printf("vypis 11\n");
								
			} else if(input_char >= 'a' && input_char <= 'f') {
								tmpx += (input_char - 'a'+10)*16;
				
									if(PRINT) printf("vypis 12\n");	
								break;						
					
			}
			throw_error(CODE_ERROR_LEX, "invalid input");
	
						
		case DIGIT:
			tmpx += (input_char - '0')*16;
			//ulozit do tmp promenne ....prevod na cislo dokoncit z hexadec. na cislo
			//neznamekovy char, nasobeni 16
			// tmp_esc_val[0] = '2';  DELETE
 			//printf("** %i\n", tmp_esc_val);
			//printf("** %c\n", tmp_esc_val);			
			//tmp_esc_val = input_char * 16;
					if(PRINT) printf("vypis 14\n");
			
			break;
		default:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;		
	}
	read_input();
	goto q17;
	
q17: //escaped string xH
	switch(input_char_type) {
		case LETTER:
		if(input_char >= 'A' && input_char <= 'F') {
				tmpx += (input_char - 'A'+10);
				break;
									
			} else if(input_char >= 'a' && input_char <= 'f') {
				tmpx += (input_char - 'a'+10);
				break;							
			}
			throw_error(CODE_ERROR_LEX, "invalid input");
			
		case DIGIT:
			tmpx += (input_char - '0');
			break;
			//tmp_esc_val[1] = '1';
			//const char *frmtChar = hexToInt(tmp_esc_val);
			//unsigned char *resChar;
			//sscanf(&frmtChar, "%c", &resChar);
		
		default:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;		
	}	
	save(&tmpData,tmpx);			
	read_input();
	goto q8;
	}

unsigned char hexToInt(unsigned char *tmp)
{
	unsigned char hp1, hp2;
	unsigned char h1 = tmp[0];
	if(h1 >= '0' && h1 <= '9') {
		hp1 = h1 - '0';
	} else if(h1 >= 'a' && h1 <= 'f') {
		hp1 = h1 - 'a' + 10;
	} else if(h1 >= 'A' && h1 <= 'F') {
		hp1 = h1 - 'A' + 10;
	}
	unsigned char h2 = tmp[1];
	if(h2 >= '0' && h2 <= '9') {
		hp2 = h2 - '0';
	} else if(h2 >= 'a' && h2 <= 'f') {
		hp2 = h2 - 'a' + 10;
	} else if(h2 >= 'A' && h2 <= 'F') {
		hp2 = h2 - 'A' + 10;
	}	
	return (16 * hp1 + hp2);
}

void save(struct lexeme *tmpData, char to_save) {
  char *holder = NULL;

  if(length>=chunks*CHUNK) { //pokud je delka retezce vetsi nez alokovane
    if(chunks)
      holder = tmpData->value.string; //napln temp
    if((tmpData->value.string = (char *)malloc(++chunks*CHUNK)) == NULL) //alokuj pamet
      throw_error(CODE_ERROR_INTERNAL, "malloc error");
    if(holder) {
      memcpy((void *)tmpData->value.string, (void *)holder, length); //kopiruj hodnotu retezce
      free(holder); //uvolni tmp
    }
  }

  tmpData->value.string[length++] = to_save; //ulozeni retezce
}

void save_temp(char to_save) {
  if(temp_length >= CHUNK) //pokud neni alokovane misto
    throw_error(CODE_ERROR_LEX, "identifier or number too long");
  temp[temp_length++] = to_save; //ulozeni retezce
}

void free_token(struct lexeme *tmpData) {
  free(tmpData->value.string);  //uvolneni pameti
}
