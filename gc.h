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

void _gc_malloc(void *x) {

}

void _gc_free(void *x) {

}

#endif //IFJ_GC_H
