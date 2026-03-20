/*
   OpenGL bitmap character and string output.

   This was originally an optimized version of the GLUT library procedure
   glutBitmapCharacter that relied on private GLUT bitmap font headers.
   Modern GLUT/freeglut packages generally do not ship those internal
   headers, so use the public API instead.

   clib v1.1

   Copyright (C) 1998 Per Kraulis
     6-Feb-1998  first attempts
*/

#include "ogl_bitmap_character.h"

#include <assert.h>


/*------------------------------------------------------------*/
void
ogl_bitmap_character (void *font, int c)
     /*
       Output the character using the given GLUT bitmap font.
       The font must be a GLUTbitmapFont.
       The raster position must be defined.
     */
{
  /* pre */
  assert (font);
  glutBitmapCharacter (font, c);
}


/*------------------------------------------------------------*/
void
ogl_bitmap_string (void *font, char *str)
     /*
       Output the string using the given GLUT bitmap font.
       The font must be a GLUTbitmapFont.
       The raster position must be defined.
     */
{
  /* pre */
  assert (font);
  assert (str);

  for ( ; *str; str++) ogl_bitmap_character (font, *str);
}
