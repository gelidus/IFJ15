#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ial.h"
#include "errors.h"
#include "common.h"
#include "string.h"
#include "gc.h"


// nova hashova tabulka co pisu ve ctvrtek den pred odevzdanim
// pak nekdo promazte tu starou

// vytvori novou tabulku
struct hash_table * create_table()
{
	struct hash_table * table = NULL;

	if(!(table = malloc(sizeof(struct hash_table)))) {
        throw_error(CODE_ERROR_INTERNAL, "malloc failure");
	}

	if(!(table->table = malloc(sizeof(struct hash_item *) * MAX_HTSIZE))) {
        throw_error(CODE_ERROR_INTERNAL, "malloc failure");
	}

    table->size = MAX_HTSIZE;

	for(int i = 0; i < table->size; i++) {
		table->table[i] = NULL;
	}
	return table;
}

// vrati hash
int make_hash(struct hash_table * hashtable, string* key)
{

	/* convert our string to an integer index */
    unsigned long int hashval = 33;
    unsigned int i = 0;
    while (hashval < ULONG_MAX && i < strlen(key->str)) {
            hashval = hashval << 8;
            hashval += key->str[i];
            i++;
    }
    return hashval % hashtable->size;

	// #ifdef HASHTABLE_USE_SIMPLE_HASH
	// for (hash = i = 0; i < key->len; hash = hash << 8, hash += key[i++]);
	// #else /* Use: Jenkins' "One At a Time Hash" === Perl "Like" Hashing */
	//  // http://en.wikipedia.org/wiki/Jenkins_hash_function
	//  for ( hash = i = 0; i < keyLength; ++i ) {
	// 	 hash += key[i], hash += ( hash << 10 ), hash ^= ( hash >> 6 );
	//  }
	//  hash += ( hash << 3 ), hash ^= ( hash >> 11 ), hash += ( hash << 15 );
	// #endif

 // 	return hash % hashtable->size;

	// unsigned long int hashval;
	// int i = 0;
    // char* key_as_string = key->str;
	//
    // // string na integer
	// while(hashval < ULONG_MAX && i < key->len) {
	// 	hashval = hashval << 8;
	// 	hashval += key_as_string[i];
	// 	i++;
	// }
	//
	// return hashval % hashtable->size;
}

// vytvori novej hash_item na vlozeni
struct hash_item * make_item(string * key, void * value)
{
	struct hash_item * new;

	if(!(new = malloc(sizeof(struct hash_item)))) {
        throw_error(CODE_ERROR_INTERNAL, "malloc failure");
	}

	new->key = new_str(key->str);
    new->value = value;
	new->next = NULL;

	return new;
}

// vlozi item
void add_item(struct hash_table * hashtable, string * key, void * value)
{
    int index = make_hash(hashtable, key);
    struct hash_item * next = hashtable->table[index];
    struct hash_item * last = NULL;
    // najdem posledni
	while(next && next->key && strcmp(key->str, next->key->str) > 0 ) {
		last = next;
		next = next->next;
	}
	// klic sedi, prepisem hodnotu
	if(next && next->key && strcmp(key->str, next->key->str) == 0 ) {
        // na free kaslem, ale kdyby nahodou :D
		// free(next->value);
		next->value = value;
    // nenasli jsme, zalozime novy
	} else {
		struct hash_item * new = make_item(key,value);
        // jsme na zacatku
		if(next == hashtable->table[index]) {
			new->next = next;
			hashtable->table[index] = new;
		// jsme na konci seznamu
		} else if (next == NULL) {
			last->next = new;
		// jsme nekde uprostred
		} else  {
			new->next = next;
			last->next = new;
		}
	}
}

// vrati hodnotu
void * get_item(struct hash_table * hashtable, string * key)
{
	int index = make_hash(hashtable, key);
    struct hash_item * ptr = NULL;
    // hledame v seznamu
	ptr = hashtable->table[index];
	while(ptr && ptr->key && strcmp(key->str, ptr->key->str) > 0) {
		ptr = ptr->next;
	}
    // nasli radcove?
	if(ptr == NULL || ptr->key == NULL || strcmp(key->str, ptr->key->str) != 0 ) {
		return NULL;
	} else {
		return ptr->value;
	}

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