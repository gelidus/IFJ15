#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ial.h"
#include "errors.h"
#include "common.h"
#include "string.h"

int HTSIZE = MAX_HTSIZE;

/**
** Inicializace tabulky.
*/
void htInit(Ttable_hash *hasht) {

   for (int i = 0; i < HTSIZE; i++)
      (*hasht)[i] = NULL;

   return;
}

/**
** Rozptylovaci funkce. Zpracovava klic
** a vraci index v rozmezí 0..HTSIZE - 1.
*/
int hashCode(char *key) {
	int retval = 1, keylen = 0;

	if (key != NULL)
	{
		keylen = strlen(key);
		for (int i = 0; i < keylen; i++)
			retval += key[i];

		return (retval % HTSIZE);
	}
	return -1;
}

/**
** Vyhleda prvek v tabulce podle klice.
** V pripade uspechu vraci data, jinak NULL.
*/
TtitemPtr htSearch(char *key, Ttable_hash *hasht) {
	// Zjisteni pozice v tabulce.
	int index = hashCode(key);
	TtitemPtr temp = (*hasht)[index];

	if (temp != NULL)
	{
		// Hledani podle klice.
		while (temp != NULL)
		{
			if(temp != NULL)
			{
				while(temp->nextPtr != NULL && temp->key != key) // Hledame klic na indexu.
					temp = temp->nextPtr; // Posuneme se dal, klic nebyl nalezen.

				if(temp->key == key)
					return temp; //Nasli jsme hledany klic, vratime dany prvek.
				else
					return NULL; // Nenasli jsme klic.
			}
		}
	}
	return NULL;
}

/**
** Vlozi prvek do seznamu.
*/
int htInsert (char *key, Ttable_hash *hasht, TData *data) {
	// Zjistim index do tabulky.
	TtitemPtr temp = htSearch(key,hasht);

	if (temp == NULL)
	{
		int index = hashCode(key);
			TtitemPtr elem;

		// Vytvoreni noveho prvku.
		if ((elem = malloc(sizeof(struct Ttitem))) == NULL)
			return CODE_ERROR_INTERNAL;
		else
		{
			strcpy(elem->key,key);	// Zkopirovani klice.
			elem->data = data;
			elem->len_key = strlen(key);
			// Novy prvek se stava prvnim a ukazuje na minuleho prvniho nebo na NULL.
			elem->nextPtr = (*hasht)[index];
			(*hasht)[index] = elem;
		}
	}
	else // Polozka existuje, aktualizuji data.
		temp->data = data;

	return 0;
}

/**
** Zrusi vsechny prvky v tabulce
*/
void htRemoveAll(Ttable_hash *hasht) {
	TtitemPtr temp, temp2;

	for (int i = 0; i < HTSIZE; i++) // Prochazime vsemi polozkami v poli.
	{
		temp = (*hasht)[i];

		while (temp != NULL) // V kazde polozce odstranime vsechny jeji prvky
		{
			temp2 = temp;
			temp = temp->nextPtr;

			if (temp2->data != NULL)
			{
				if (temp2->data->type == T_STR) // Musime take uvolnit retezec, pokud je.
					free(temp2->data->val.str);
				else if (temp2->data->type == T_FUNC) // Stejne tak lokalni hashovaci tabulku.
				{
					if (temp2->data->val.vlFunc.loc_hash != NULL)
					{
						htRemoveAll((Ttable_hash*)temp2->data->val.vlFunc.loc_hash);
						free((Ttable_hash*)temp2->data->val.vlFunc.loc_hash);
					}
				}
				free(temp2->data);
			}
			free(temp2->key);
			free(temp2);
		}
		(*hasht)[i]=NULL;
	}

	return;
}

/**
** Najde a vrati data prvku.
*/
TData *htRead(char *key, Ttable_hash *hasht) {
	TtitemPtr search = htSearch(key,hasht);

	if (search != NULL)
		return search->data;

	return NULL;
}

int length(char* s) {
	int c = 0;
	if (!strcmp(s, "")) {
		return 0;
	}
 	while (s[c] != '\0')
  	c++;
 	return c;
}

char* substr(char* s, int i, int n) {
  int max;
  if(length(s) > i+n)
    max = i+n-1;
  else
    max = length(s)-1;
  string* str = new_str("");
  for (i; i <= max; i++) {
    add_char(str,s[i]);
  }
  return str->str;
}

char* concat(char* s1, char* s2) {
  return (cat_str(new_str(s1), new_str(s2))->str);
}

/**sort**/
char* sort(char* input)
{
	int c = strlen(input);
	char* pom = (char *)malloc(c);
	memcpy((void *)pom, (void *)input, c);

	int j, tmp;
	for(int inc = c/2; inc > 0; inc = inc/2) {
		for(int i = inc; i<c; i++) {
			tmp = pom[i];
			for(j = i; j >= inc ;j = j-inc) {
				if(tmp < pom[j-inc]) {
					pom[j] = pom[j-inc];
				} else {
					break;
				}
			}
			pom[j] = tmp;
		}
	}
	return pom;
}
/**sort**/

/**find**/
#define MAX 256

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void heur(char *str, int chr[MAX], int size)
{
    int i;
    for (i = 0; i < MAX; i++) {
		chr[i] = -1;
	}
    for (i = 0; i < size; i++) {
        chr[(int)str[i]] = i;
	}
}

int find(char *input, char *pat)
{
    int size_pat = strlen(pat);
    int n = strlen(input);
    int chr[MAX];

    heur(pat, chr, size_pat);

    int tmp = 0;
    while (tmp <= (n - size_pat))
    {
        int a = size_pat - 1;
        while (a >= 0 && pat[a] == input[tmp+a]) {
			a--;
		}

        if (a < 0)
        {
            return tmp;
        } else {
            tmp += max(1, a - chr[(int)input[tmp+a]]);
		}
    }
    return -1;
}
/**find**/
