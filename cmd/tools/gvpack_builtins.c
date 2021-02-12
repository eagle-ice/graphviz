/* vim:set shiftwidth=4 ts=8: */

/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at http://www.graphviz.org/
 *************************************************************************/

#include <gvc/gvplugin.h>

/*visual studio*/
#ifdef _MSC_VER
#define extern __declspec(dllimport)
#endif
/*end visual studio*/
extern gvplugin_library_t gvplugin_neato_layout_LTX_library;

lt_symlist_t lt_preloaded_symbols[] = {
	{ "gvplugin_neato_layout_LTX_library", (void*)(&gvplugin_neato_layout_LTX_library) },
	{ 0, 0 }
};
