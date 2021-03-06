/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifndef GV_MEMORY_H
#define GV_MEMORY_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NEW(t)           (t*)zmalloc(sizeof(t))
#define N_NEW(n,t)       (t*)gcalloc((n),sizeof(t))
#define GNEW(t)          (t*)gmalloc(sizeof(t))

#define N_GNEW(n,t)      (t*)gcalloc((n),sizeof(t))
#define N_GGNEW(n,t)      (t*)calloc((n),sizeof(t))
#define ALLOC(size,ptr,type) (ptr? (type*)grealloc(ptr,(size)*sizeof(type)):(type*)gmalloc((size)*sizeof(type)))
#define RALLOC(size,ptr,type) ((type*)grealloc(ptr,(size)*sizeof(type)))
#define ZALLOC(size,ptr,type,osize) (ptr? (type*)zrealloc(ptr,size,sizeof(type),osize):(type*)zmalloc((size)*sizeof(type)))
#ifdef GVDLL
#define extern __declspec(dllexport)
#else
#ifdef _WIN32
#ifndef GVC_EXPORTS
#define extern __declspec(dllimport)
#endif
#endif

#endif

    extern void *zmalloc(size_t);
    extern void *zrealloc(void *, size_t, size_t, size_t);
    extern void *gcalloc(size_t nmemb, size_t size);
    extern void *gmalloc(size_t);
	extern void *grealloc(void *, size_t);
#undef extern

#ifdef __cplusplus
}
#endif

#endif
