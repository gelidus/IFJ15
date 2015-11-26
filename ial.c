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
	unsigned long int hashval;
	int i = 0;
    char* key_as_string = key->str;

    // string na integer
	while(hashval < ULONG_MAX && i < key->len) {
		hashval = hashval << 8;
		hashval += key_as_string[i];
		i++;
	}

	return hashval % hashtable->size;
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


int HTSIZE = MAX_HTSIZE;

// hash table functions
unsigned long Hash(char* what) {
  unsigned long hash = 5381;
  int c;

  while ((c = *what++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % HTSIZE;
}

void SymbolTableInit() {
  // initialize the array of pointers. Each pointer
  // will be then initialized by the table when created
  SymbolTable = (Scope**)gc_malloc(MAX_HTSIZE * sizeof(Scope*));

  // NULL all the pointers in the symbol table, so we
  // can work with null pointers (remove trash)
  for (int i = 0; i < MAX_HTSIZE; i++) {
    SymbolTable[i] = NULL;
  }
}

Variable* SymbolTableAddVariable(string* scope_name, struct ast_node* var) {
  unsigned long hash = Hash(scope_name->str);

  Scope* scope = SymbolTableFindScope(scope_name);
  if (scope == NULL) {
    scope = SymbolTableCreateScope(scope_name);
    if (scope == NULL) {
      throw_error(CODE_ERROR_INTERNAL, "[SymbolTable] Scope could not be created");
    }
  }

  // add variable to the scope that was created
  return ScopeAddVariable(scope, var);
}

Variable* SymbolTableFindVariable(string* scope_name, string* var_name) {
  unsigned long hash = Hash(scope_name->str);

  Scope* scope = SymbolTableFindScope(scope_name);
  if (scope == NULL) {
     return NULL; // not even scope for the variable was found
  }

  // try to search the scope for the variable
  return ScopeFindVariable(scope, var_name);
}

Scope* SymbolTableFindScope(string* scope_name) {
  unsigned long hash = Hash(scope_name->str);

  Scope* scope_list = SymbolTable[hash];
  // the scope list was not found
  if (scope_list == NULL) {
    return NULL;
  }

  Scope* scope = scope_list;
  while (scope != NULL) {
    if (equals(scope->name, scope_name) == true) {
      break; // found scope
    }

    // else continue finding
    scope = scope->next;
  }

  return scope;
}

Scope* SymbolTableCreateScope(string* scope_name) {
  Scope* scope = (Scope*)gc_malloc(sizeof(Scope*));
  scope->name = scope_name;
  scope->first = NULL;
  scope->next = NULL;

  unsigned long hash = Hash(scope_name->str);

  Scope* insert = SymbolTable[hash];
  if (insert == NULL) {
    // insert the first scope
    SymbolTable[hash] = scope;
  } else {
    // interate to the end of the list
    while (insert->next != NULL) {
      insert = insert->next;
    }
    // insert to the end of the list
    insert->next = scope;
  }

  return scope; // no error
}

Variable* ScopeAddVariable(Scope* scope, struct ast_node* node) {

  Variable* var = (Variable*)gc_malloc(sizeof(Variable*));
  var->var = node;
  var->next = NULL;

  if (scope->first == NULL) {
    scope->first = var;
  } else {
    Variable* insert = scope->first;
    while(insert->next != NULL) {
      insert = insert->next;
    }

    insert->next = var;
  }

  return var;
}

Variable* ScopeFindVariable(Scope* scope, string* var_name) {
  Variable* var = scope->first;

  while (var != NULL) {
    // match the variable by name
    if (equals(var->var->d.string_data, var_name)) {
      break;
    }

    var = var->next;
  }

  return var;
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
