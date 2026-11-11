/* Standalone stroke-font tables; see the notices in stroke_*roman.c. */
#ifndef MOLSCRIPT_STROKE_FONTS_H
#define MOLSCRIPT_STROKE_FONTS_H
#include <stddef.h>

typedef struct { float x, y; } ms_stroke_vertex;
typedef struct {
  int count;
  const ms_stroke_vertex *vertices;
} ms_stroke_strip;
typedef struct {
  float advance;
  int count;
  const ms_stroke_strip *strips;
} ms_stroke_char;
typedef struct {
  const char *name;
  int count;
  float height;
  const ms_stroke_char *const *characters;
} ms_stroke_font;

extern const ms_stroke_font ms_stroke_roman, ms_stroke_mono_roman;
#endif
