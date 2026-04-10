/* image.h

   MolScriptSVG v2.1.5

   Image file output definitions and routines.

   This implementation uses OpenGL rendering from 'opengl.c'. It prefers
   a headless EGL context for image output and falls back to GLUT if EGL
   initialization fails.

   Copyright (C) 1997-1998 Per Kraulis
    11-Sep-1997  split out of jpeg.h
*/

#ifndef IMAGE_H
#define IMAGE_H 1

void image_first_plot (void);
void image_render (void);
void image_close (void);

#endif
