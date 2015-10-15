//
// Created by Peter on 10/10/2015.
//

#ifndef IFJ_GC_H
#define IFJ_GC_H

#ifndef DEBUG // define what if release
	#define gc_malloc(x) _gc_malloc(x)
	#define gc_free(x) _gc_free(x)
#else
	#define gc_malloc(x)
	#define gc_free(x)
#endif

void* _gc_malloc(int size) {
	return malloc(size);
}

void _gc_free() {
	// nebudeme zatim volat
}

#endif //IFJ_GC_H
