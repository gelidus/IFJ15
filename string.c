/*
    IFJ 2013

    xmarko07 - AntonÃ­n Marko
    xmahne00 - Jakub Mahnert
    xkurit00 - Jakub KuÅ™itka
    xkubic34 - Martin KubÃ­Äek
    xlechp00  - Pali Lech

    "Let the C language die."
 */

//jednoducha knihovna pro praci s nekonecne dlouhymi retezci
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "str.h"
#include "gc.h"

#define STR_LEN_INC 8
// konstanta STR_LEN_INC udava, na kolik bytu provedeme pocatecni alokaci pameti
// pokud nacitame retezec znak po znaku, pamet se postupne bude alkokovat na
// nasobky tohoto cisla


string* new_str(char* txt)
// funkce vytvori novy retezec
{
	string* s = (string*)gc_malloc(sizeof(string));
	if (!s) {
		return NULL;
	}

    if ((s->str = (char*)gc_malloc(sizeof(char) * STR_LEN_INC)) == NULL) {
        // malloc error
        return NULL;
    }


	if (!txt) {
		s->str[0] = '\0';
		s->len = 0;
		s->alloc_size = STR_LEN_INC;
	} else {
		s->len = 0;
		convert_chars(s, txt);
	}

	return s;
}

// funkce vymaze obsah retezce
void clear_str(string*s) {
   s->str[0] = '\0';
   s->len = 0;
}

// spoji dva stringy a vrati spojeny
string* cat_str(string* str1, string* str2) {
   string* str = new_str(str1->str);
   convert_chars(str, str2->str);

   return str;
}

// prida na konec retezce jeden znak
int add_char(string* s, char c) {
   if (s->len + 1 >= s->alloc_size) {
      // pamet nestaci, je potreba provest realokaci
      if ((s->str = (char*)gc_realloc(s->str, (s->len + STR_LEN_INC) * sizeof(char))) == NULL)
         return false;
      s->alloc_size = s->len + STR_LEN_INC;
   }

   s->str[s->len] = c;
   s->len++;
   s->str[s->len] = '\0';

   return true;
}

// konvertuje pole znaku do struktury
int convert_chars(string* s, char* chs) {
	if (s != NULL && chs != NULL) {
		int i = 0;

		while(chs[i] != '\0'){
			add_char(s, chs[i]);
			i++;
		}

		return true;
	}

	return false;
}

int equals(string* s1, string* s2) {
	if(s1->len == s2->len) {
		for(int i = 0; i < s1->len; i++) {
			if(s1->str[i] != s2->str[i])
				return 0; //nalezen rozdil
		}

		//probehlo v poradku
		return true;
	}

	//nejsou stejne delky = rozdilne stringy
	return false;
}
