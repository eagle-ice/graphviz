/*************************************************************************
 * Copyright (c) 2014 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at https://graphviz.org
 *************************************************************************/

#ifndef LAB_GAMUT_H
#define LAB_GAMUT_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(LAB_GAMUT_EXPORTS)
#define EXTERN __declspec(dllimport)
#else
#define EXTERN /* nothing */
#endif

EXTERN extern const signed char lab_gamut_data[];
EXTERN extern int lab_gamut_data_size;

#undef EXTERN

#ifdef __cplusplus
}
#endif

#endif
