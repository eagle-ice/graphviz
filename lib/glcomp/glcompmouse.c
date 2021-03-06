/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include <glcomp/glcompmouse.h>
#include <glcomp/glcompfont.h>
#include <glcomp/glcompset.h>
#include <glcomp/glutils.h>

void glCompMouseInit(glCompMouse * m)
{
    m->functions.click = glCompClick;
    m->functions.doubleclick = glCompDoubleClick;
    m->functions.draw = NULL;
    m->functions.mousedown = glCompMouseDown;
    m->functions.mousedrag = glCompMouseDrag;
    m->functions.mousein = glCompMouseIn;
    m->functions.mouseout = glCompMouseOut;
    m->functions.mouseover = glCompMouseOver;
    m->functions.mouseup = glCompMouseUp;

    m->callbacks.click = NULL;
    m->callbacks.doubleclick = NULL;
    m->callbacks.draw = NULL;
    m->callbacks.mousedown = NULL;
    m->callbacks.mousedrag = NULL;
    m->callbacks.mousein = NULL;
    m->callbacks.mouseout = NULL;
    m->callbacks.mouseover = NULL;
    m->callbacks.mouseup = NULL;
    m->dragX = 0;
    m->dragY = 0;
    m->down = 0;

}
extern void glCompClick(glCompObj * o, GLfloat x, GLfloat y,
			glMouseButtonType t)
{
    (void)o;
    (void)x;
    (void)y;
    (void)t;
}
extern void glCompDoubleClick(glCompObj * obj, GLfloat x, GLfloat y,
			      glMouseButtonType t)
{
    (void)obj;
    (void)x;
    (void)y;
    (void)t;
}

extern void glCompMouseDown(glCompObj * obj, GLfloat x, GLfloat y,
			    glMouseButtonType t)
{
    (void)obj;
    (void)x;
    (void)y;
    (void)t;
}

extern void glCompMouseIn(glCompObj * obj, GLfloat x, GLfloat y)
{
    (void)obj;
    (void)x;
    (void)y;
}
extern void glCompMouseOut(glCompObj * obj, GLfloat x, GLfloat y)
{
    (void)obj;
    (void)x;
    (void)y;
}
extern void glCompMouseOver(glCompObj * obj, GLfloat x, GLfloat y)
{
    (void)obj;
    (void)x;
    (void)y;
}
extern void glCompMouseUp(glCompObj * obj, GLfloat x, GLfloat y,
			  glMouseButtonType t)
{
    (void)obj;
    (void)x;
    (void)y;
    (void)t;
}
extern void glCompMouseDrag(glCompObj * obj, GLfloat dx, GLfloat dy,
			    glMouseButtonType t)
{
    (void)obj;
    (void)dx;
    (void)dy;
    (void)t;
}
