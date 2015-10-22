#include "scanner.h"

const static struct {
  char* id;
  enum lex_type type;
} keywords_table[] = {
  {"auto", KW_AUTO}, {"cin", KW_CIN}, {"cout", KW_COUT}, {"double", KW_DOUBLE}, {"else", KW_ELSE},
  {"for", KW_FOR}, {"if", KW_IF}, {"int", KW_INT}, {"return", KW_RETURN}, {"string", KW_STRING},
  {NULL, NO_TYPE}
};

static int input_char; //prave nacteny znak
static enum literal_type input_char_type; //prave nacteny typ znaku

static unsigned char temp[CHUNK];
//static char* safeStr = "74|75|99|111|112|121|114|105|103|104|116";
static size_t temp_length;

static void save(struct lexeme *, unsigned char); //trvale ulozeni retezce do lexemu
static void save_temp(unsigned char); //docasne ulozeni stringu

static size_t length; //delka retezce
static int chunks; //zvetsovani bufferu

static char *e_strtod; //osetreni parametru pro prevod cisel double

enum literal_type check_keyword(unsigned char* candidate) {
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

void get_token()
{
    struct lexeme lex = read_lexeme();

}

//hlavni funkce
struct lexeme read_lexeme(void) {
	struct lexeme tmpData;
	tmpData.type = NO_TYPE;

	read_input();

q0: //default state
	switch(input_char_type) {
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
		case UNDERSCORE:
			temp_length = 0;
			save_temp(input_char);
			read_input();
			goto q2; //underscored identifier
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
			save_temp(0);
			if((tmpData.type = check_keyword(temp)) == IDENTIFIER) {
				if((tmpData.value.string = (unsigned char *)malloc(temp_length)) == NULL)
					throw_error(ERR_INT, "malloc error");
				memcpy((void *)tmpData.value.string, (void *)temp, temp_length);
			}
			return tmpData;
	}

q2: //underscored identifier OK
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

q3: //comments or divide OK
	switch(input_char_type) {
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
							tmpData.type = END_OF_FILE;
							return tmpData;
						}
						read_input();
						if(input_char == '/') {
							read_input();
							goto q0; //default state
						}
					} while(1);
				default:
					throw_error(CODE_ERROR_LEX, "invalid input");
					break;
			}
			break;
	}

q4: //integer or double OK
	switch(input_char_type) {
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q4; //integer or double
		case WHITE_SPACE:
			save_temp(0);
			tmpData.value.integer = atoi((const char *)temp);
			return tmpData;
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
			return_input();
			save_temp(0);
			tmpData.value.integer = atoi((const char *)temp);
			return tmpData;
		case OTHERS:
			if(input_char == '.') {
				save_temp(input_char);
				read_input();
				tmpData.type = DOUBLE;
				goto q6; //double
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
	switch(input_char_type) {
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q5; //double e
		case OTHERS:
			if(input_char == '+' || input_char == '-') {
				save_temp(input_char);
				read_input();
				goto q7; //double e +-
			} else {
				return_input();
				save_temp(0);
				tmpData.value.real = strtod((const char *)temp, &e_strtod);
				return tmpData;
			}
		case WHITE_SPACE:
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
		case END:
			return_input();
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
		default:
			return_input();
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
	}

q6: //double OK
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
				return_input();
				save_temp(0);
				tmpData.value.real = strtod((const char *)temp, &e_strtod);
				return tmpData;
			}
		case WHITE_SPACE:
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
		case END:
			return_input();
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
		default:
			return_input();
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
	}

q7: //double e +- OK
	switch(input_char_type) {
		case DIGIT:
			save_temp(input_char);
			read_input();
			goto q7; //double e +-
		case WHITE_SPACE:
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
		case END:
			return_input();
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
		default:
			return_input();
			save_temp(0);
			tmpData.value.real = strtod((const char *)temp, &e_strtod);
			return tmpData;
	}

q8: //string OK
	switch(input_char_type) {
		case OTHERS:
			if(input_char == '\\') {
				save(&tmpData, input_char);
				read_input();
				goto q9;	//escaped string
			} else if(input_char == '"') {
				//q10
				save(&tmpData, 0);
				return tmpData;
			}
			save(&tmpData, input_char);
			read_input();
			goto q8; //string
		case END:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;
		case WHITE_SPACE:
			if(input_char == '\n') {
				throw_error(CODE_ERROR_LEX, "invalid input");
				break;
			}
		default:
			save(&tmpData, input_char);
			read_input();
			goto q8;  //string
	}

q9: //escaped string OK
	switch(input_char_type) {
		case END:
			throw_error(CODE_ERROR_LEX, "invalid input");
			break;
		case WHITE_SPACE:
			if(input_char == '\n') {
				throw_error(CODE_ERROR_LEX, "invalid input");
				break;
			}
		default:
			save(&tmpData, input_char);
			read_input();
			goto q8;  //string
	}

	return tmpData;
}

void save(struct lexeme *tmpData, unsigned char to_save) {
  unsigned char *holder = NULL;

  if(length>=chunks*CHUNK) { //pokud je delka retezce vetsi nez alokovane
    if(chunks)
      holder = tmpData->value.string; //napln temp
    if((tmpData->value.string = (unsigned char *)malloc(++chunks*CHUNK)) == NULL) //alokuj pamet
      throw_error(ERR_INT, "malloc error");
    if(holder) {
      memcpy((void *)tmpData->value.string, (void *)holder, length); //kopiruj hodnotu retezce
      free(holder); //uvolni tmp
    }
  }

  tmpData->value.string[length++] = to_save; //ulozeni retezce
}

void save_temp(unsigned char to_save) {
  if(temp_length >= CHUNK) //pokud neni alokovane misto
    throw_error(CODE_ERROR_LEX, "identifier or number too long");
  temp[temp_length++] = to_save; //ulozeni retezce
}

void free_token(struct lexeme *tmpData) {
  free(tmpData->value.string);  //uvolneni pameti
}
