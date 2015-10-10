//hlavickovy soubor pro praci s nekonecne dlouhymi retezci
#ifndef STR_H
#define STR_H
typedef struct
{
  char* str;		// misto pro dany retezec ukonceny znakem '\0'
  int len;		// skutecna delka retezce
  int alloc_size;	// velikost alokovane pameti
} string;


string* new_str(char* txt);
void clear_str(string* s);
int concatenate(string* s1, string* s2);
int add_char(string* s, char c);
string* cat_str(string* s1, string* s2);
int convert_chars(string* s, char* chs);
int equals(string* s1, string* s2);
#endif
