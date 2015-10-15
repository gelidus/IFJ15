//
// Created by Peter on 10/10/2015.
//

#ifndef IFJ_GC_H
#define IFJ_GC_H

// #define gc_malloc(x) //_gc_malloc(x)
// #define gc_free(x) //_gc_free(x)

// #ifndef DEBUG // define what if release
//
// #else
// 	#define gc_malloc(x)
// 	#define gc_free(x)
// #endif
//
void* gc_malloc(int size) {
	return malloc(size);
}
//
void gc_free(void*  x) {
	// nebudeme zatim volat
}

#endif //IFJ_GC_H
