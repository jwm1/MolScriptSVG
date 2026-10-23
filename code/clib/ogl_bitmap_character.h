#ifndef OGL_BITMAP_CHARACTER_H
#define OGL_BITMAP_CHARACTER_H 1

#include <GL/glut.h>

/* NULL on success; otherwise a diagnostic suitable for the command line. */
const char *ogl_font_configure (const char *mode, const char *filename);

void
ogl_bitmap_character (void *font, int c);

void
ogl_bitmap_string (void *font, char *str);

#endif
