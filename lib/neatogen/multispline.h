/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifndef MULTISPLINE_H
#define MULTISPLINE_H

#include <render.h>
#include <pathutil.h>

typedef struct router_s router_t;

extern void freeRouter (router_t* rtr);
extern router_t* mkRouter (Ppoly_t** obs, int npoly);
extern int makeMultiSpline(graph_t* g, edge_t* e, router_t * rtr, int);

#endif
