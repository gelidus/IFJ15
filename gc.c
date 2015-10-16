//
// Created by Peter on 10/16/2015.
//

#include "gc.h"
#include <malloc.h>

void* gc_malloc(int size) {
	return malloc((size_t)size);
}

void gc_free(void* x) {
	// nebudeme zatim volat
}