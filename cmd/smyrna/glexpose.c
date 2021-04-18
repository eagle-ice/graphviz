/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/
#include "glexpose.h"
#include "draw.h"
#include "topviewfuncs.h"
#include <glcomp/glutils.h>
#include "topfisheyeview.h"
#include "gui/toolboxcallbacks.h"
#include "arcball.h"
#include "hotkeymap.h"
#include "polytess.h"

GLuint texture[3];

static void drawRotatingAxis(void)
{
    static GLUquadricObj *quadratic;
    float AL = 45;

    if (get_mode(view) != MM_ROTATE)
	    return;

    if (!quadratic) {
	quadratic = gluNewQuadric();	// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals
	gluQuadricDrawStyle(quadratic, GLU_LINE);


    }

	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf(view->arcball->Transform.M);	/*arcball transformations , experimental */
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1, 1, 0);

	glVertex3f(0, 0, 0);
	glVertex3f(0, AL, 0);

	glVertex3f(0, 0, 0);
	glVertex3f(AL, 0, 0);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, AL);

	glEnd();
	glColor4f(0, 1, 0, 0.3f);
	gluSphere(quadratic, AL, 20, 20);
	glLineWidth(1);
	glPopMatrix();


}



/*
	refreshes camera settings using view parameters such as pan zoom etc
	if a camera is selected viewport is switched to 3D
	params:ViewInfo	, global view variable defined in viewport.c
	return value:always 1
*/
static int glupdatecamera(ViewInfo * vi)
{
    if (vi->active_camera == -1)
	glTranslatef(-vi->panx, -vi->pany, vi->panz);


    /*toggle to active camera */
    else {
	glMultMatrixf(vi->arcball->Transform.M);	/*arcball transformations , experimental */
	glTranslatef(-vi->cameras[vi->active_camera]->targetx,
		     -vi->cameras[vi->active_camera]->targety, 0);
    }
    vi->clipX1=0;
    vi->clipX2=0;
    vi->clipY1=0;
    vi->clipY2=0;
    vi->clipZ1=0;
    vi->clipZ2=0;
    GetOGLPosRef(1, vi->h - 5, &(vi->clipX1), &(vi->clipY1),
		 &(vi->clipZ1));
    GetOGLPosRef(vi->w - 1, 1, &(vi->clipX2), &(vi->clipY2),
		 &(vi->clipZ2));

    if (vi->active_camera == -1) {
	glScalef(1 / vi->zoom * -1, 1 / vi->zoom * -1,
		 1 / vi->zoom * -1);
    } else {
	glScalef(1 / vi->cameras[vi->active_camera]->r,
		 1 / vi->cameras[vi->active_camera]->r,
		 1 / vi->cameras[vi->active_camera]->r);
    }


    return 1;
}

/*
	draws grid (little dots , with no use)
	params:ViewInfo	, global view variable defined in viewport.c
	return value:none
*/
static void glexpose_grid(ViewInfo * view)
{
    //drawing grids
    float x, y;
    if (view->gridVisible) {
	glPointSize(1);
	glBegin(GL_POINTS);
	glColor4f(view->gridColor.R, view->gridColor.G, view->gridColor.B,
		  view->gridColor.A);
	for (x = view->bdxLeft; x <= view->bdxRight;
	     x = x + view->gridSize) {
	    for (y = view->bdyBottom; y <= view->bdyTop;
		 y = y + view->gridSize) {
		glVertex3f(x, y, 0);
	    }
	}
	glEnd();
    }
}

/*
	draws active graph depending on graph type
	params:ViewInfo	, global view variable defined in viewport.c
	return value:1 if there is a graph to draw else 0 
*/
static int glexpose_drawgraph(ViewInfo * view)
{

    if (view->activeGraph > -1) {
	if (!view->Topview->fisheyeParams.active)
	    renderSmGraph(view->g[view->activeGraph],view->Topview);	    
	else {
	    drawtopologicalfisheye(view->Topview);
	}

	return 1;
    }
    return 0;
}

/*
	main gl expose ,any time sreen needs to be redrawn, this function is called by gltemplate
	,all drawings are initialized in this function
	params:ViewInfo	, global view variable defined in viewport.c
	return value:0 if something goes wrong with GL 1 , otherwise
*/
int glexpose_main(ViewInfo * view)
{
    static int doonce = 0;
    if (!glupdatecamera(view))
	return 0;

    if (view->activeGraph >= 0) {
	if (!doonce) {
	    doonce = 1;
	    btnToolZoomFit_clicked(NULL, NULL);
	    btnToolZoomFit_clicked(NULL, NULL);
	}
    }
    else
	return 0;



    glexpose_grid(view);
    drawBorders(view);
    glexpose_drawgraph(view);
    drawRotatingAxis();
    draw_selpoly(&view->Topview->sel.selPoly);
    glCompSetDraw(view->widgets);

	 /*DEBUG*/ return 1;
}
