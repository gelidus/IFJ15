#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ial.h"
#include "errors.h"
#include "common.h"
#include "string.h"

int HTSIZE = MAX_HTSIZE;

// hash table functions
unsigned long Hash(unsigned char *what) {
  unsigned long hash = 5381;
  int c;

  while ((c = *what++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % HTSIZE;
}

void HashTableInit() {

}

void HashTableAddVariable(string *scope, string *var_name, enum ast_var_type type) {

}

Variable* HashTableFindVariable(string *scope, string *var_name) {

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
