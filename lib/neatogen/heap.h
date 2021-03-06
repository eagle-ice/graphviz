/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif



#ifndef HEAP_H
#define HEAP_H

#include <neatogen/hedges.h>

    extern void PQinitialize(void);
    extern void PQcleanup(void);
    extern Halfedge *PQextractmin(void);
    extern Point PQ_min(void);
    extern int PQempty(void);
    extern void PQdelete(Halfedge *);
    extern void PQinsert(Halfedge *, Site *, double);

#endif

#ifdef __cplusplus
}
#endif
