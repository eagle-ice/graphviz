/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifndef POLYTESS_H
#define POLYTESS_H
#include "smyrnadefs.h"

#ifdef __cplusplus
extern "C" {
#endif
    typedef struct {
	GLUtesselator *tobj;
	GLenum windingRule;
    }tessPoly;
    extern int drawTessPolygon(sdot_op* p);
    extern int testDraw();
#ifdef __cplusplus
}				/* end extern "C" */
#endif
#endif
