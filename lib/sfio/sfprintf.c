/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#include	<sfio/sfhdr.h>

/*	Print data with a given format
**
**	Written by Kiem-Phong Vo.
*/

int sfprintf(Sfio_t * f, const char *form, ...)
{
    va_list args;
    int rv;

    va_start(args, form);
    rv = sfvprintf(f, form, args);

    va_end(args);
    return rv;
}

int sfvsprintf(char *s, int n, const char *form, va_list args)
{
    Sfio_t f;
    int rv;

    if (!s || n <= 0)
	return -1;

    /* make a fake stream */
    SFCLEAR(&f);
    f.flags = SF_STRING | SF_WRITE;
    f.mode = SF_WRITE;
    f.size = n - 1;
    f.data = f.next = f.endr = (uchar *) s;
    f.endb = f.endw = f.data + f.size;

    rv = sfvprintf(&f, form, args);
    *f.next = '\0';
    _Sfi = f.next - f.data;

    return rv;
}

int sfsprintf(char *s, int n, const char *form, ...)
{
    va_list args;
    int rv;
    va_start(args, form);
    rv = sfvsprintf(s, n, form, args);
    va_end(args);

    return rv;
}
