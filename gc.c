//
// Created by Peter on 10/16/2015.
//

#include "gc.h"
// malloc.h is non-standard linux-specific header.
// malloc() function always exists in stdlib.h - it's guaranteed by standard.
// https://code.google.com/p/word2vec/issues/detail?id=17
// #include<malloc.h>
#include <stdlib.h>

void* gc_malloc(int size) {
	return malloc((size_t)size);
}

void gc_free(void* x) {
	// nebudeme zatim volat
}
