/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/
#ifndef GLCOMPLABEL_H
#define GLCOMPLABEL_H

#include <glcomp/glcompdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

    extern glCompLabel *glCompLabelNew(glCompObj * par, GLfloat x,
				       GLfloat y, char *text);
    extern int glCompSetAddLabel(glCompSet * s, glCompLabel * p);
    extern int glCompSetRemoveLabel(glCompSet * s, glCompLabel * p);

/*events*/
    extern int glCompLabelDraw(glCompLabel * p);
    extern void glCompLabelClick(glCompObj * o, GLfloat x, GLfloat y,
				 glMouseButtonType t);
    extern void glCompLabelDoubleClick(glCompObj * obj, GLfloat x,
				       GLfloat y, glMouseButtonType t);
    extern void glCompLabelMouseDown(glCompObj * obj, GLfloat x, GLfloat y,
				     glMouseButtonType t);
    extern void glCompLabelMouseIn(glCompObj * obj, GLfloat x, GLfloat y);
    extern void glCompLabelMouseOut(glCompObj * obj, GLfloat x, GLfloat y);
    extern void glCompLabelMouseOver(glCompObj * obj, GLfloat x,
				     GLfloat y);
    extern void glCompLabelMouseUp(glCompObj * obj, GLfloat x, GLfloat y,
				   glMouseButtonType t);

    extern void glCompLabelSetText(glCompLabel * p, char *text);
    extern void glCompLabelSetFontSize(glCompLabel * p, int size);
    extern void glCompLabelSetFontName(glCompLabel * p, char* fontName);

#ifdef __cplusplus
}
#endif
#endif
