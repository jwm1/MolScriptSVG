/* Display-independent OpenGL labels. Copyright (C) 2026 James W. Murray.
   Original interface: Copyright (C) 1998 Per Kraulis. See LICENSE.txt. */
#include "ogl_bitmap_character.h"
#include "str_utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef FREETYPE_SUPPORT
#include <ft2build.h>
#include FT_FREETYPE_H
static FT_Library library;
static FT_Face face;
#endif

/* Bundled 5x7 ASCII font. Rows run top to bottom; bit 4 is the left pixel. */
static const unsigned char glyphs[95][7] = {
  ['!'-32]={4,4,4,4,4,0,4}, ['"'-32]={10,10,10,0,0,0,0},
  ['#'-32]={10,31,10,10,31,10,0}, ['$'-32]={4,15,20,14,5,30,4},
  ['%'-32]={24,25,2,4,8,19,3}, ['&'-32]={12,18,20,8,21,18,13},
  ['\''-32]={4,4,8,0,0,0,0}, ['('-32]={2,4,8,8,8,4,2},
  [')'-32]={8,4,2,2,2,4,8}, ['*'-32]={0,21,14,31,14,21,0},
  ['+'-32]={0,4,4,31,4,4,0}, [','-32]={0,0,0,0,4,4,8},
  ['-'-32]={0,0,0,31,0,0,0}, ['.'-32]={0,0,0,0,0,12,12},
  ['/'-32]={1,2,2,4,8,8,16},
  ['0'-32]={14,17,19,21,25,17,14}, ['1'-32]={4,12,4,4,4,4,14},
  ['2'-32]={14,17,1,2,4,8,31}, ['3'-32]={30,1,1,14,1,1,30},
  ['4'-32]={2,6,10,18,31,2,2}, ['5'-32]={31,16,16,30,1,1,30},
  ['6'-32]={6,8,16,30,17,17,14}, ['7'-32]={31,1,2,4,8,8,8},
  ['8'-32]={14,17,17,14,17,17,14}, ['9'-32]={14,17,17,15,1,2,12},
  [':'-32]={0,12,12,0,12,12,0}, [';'-32]={0,12,12,0,4,4,8},
  ['<'-32]={1,2,4,8,4,2,1}, ['='-32]={0,0,31,0,31,0,0},
  ['>'-32]={16,8,4,2,4,8,16}, ['?'-32]={14,17,1,2,4,0,4},
  ['@'-32]={14,17,23,21,23,16,14},
  ['A'-32]={14,17,17,31,17,17,17}, ['B'-32]={30,17,17,30,17,17,30},
  ['C'-32]={14,17,16,16,16,17,14}, ['D'-32]={30,17,17,17,17,17,30},
  ['E'-32]={31,16,16,30,16,16,31}, ['F'-32]={31,16,16,30,16,16,16},
  ['G'-32]={14,17,16,23,17,17,15}, ['H'-32]={17,17,17,31,17,17,17},
  ['I'-32]={14,4,4,4,4,4,14}, ['J'-32]={7,2,2,2,2,18,12},
  ['K'-32]={17,18,20,24,20,18,17}, ['L'-32]={16,16,16,16,16,16,31},
  ['M'-32]={17,27,21,21,17,17,17}, ['N'-32]={17,25,21,19,17,17,17},
  ['O'-32]={14,17,17,17,17,17,14}, ['P'-32]={30,17,17,30,16,16,16},
  ['Q'-32]={14,17,17,17,21,18,13}, ['R'-32]={30,17,17,30,20,18,17},
  ['S'-32]={15,16,16,14,1,1,30}, ['T'-32]={31,4,4,4,4,4,4},
  ['U'-32]={17,17,17,17,17,17,14}, ['V'-32]={17,17,17,17,17,10,4},
  ['W'-32]={17,17,17,21,21,27,17}, ['X'-32]={17,17,10,4,10,17,17},
  ['Y'-32]={17,17,10,4,4,4,4}, ['Z'-32]={31,1,2,4,8,16,31},
  ['['-32]={14,8,8,8,8,8,14}, ['\\'-32]={16,8,8,4,2,2,1},
  [']'-32]={14,2,2,2,2,2,14}, ['^'-32]={4,10,17,0,0,0,0},
  ['_'-32]={0,0,0,0,0,0,31}, ['`'-32]={8,4,2,0,0,0,0},
  ['a'-32]={0,0,14,1,15,17,15}, ['b'-32]={16,16,30,17,17,17,30},
  ['c'-32]={0,0,14,17,16,17,14}, ['d'-32]={1,1,15,17,17,17,15},
  ['e'-32]={0,0,14,17,31,16,14}, ['f'-32]={6,8,8,30,8,8,8},
  ['g'-32]={0,15,17,17,15,1,14}, ['h'-32]={16,16,30,17,17,17,17},
  ['i'-32]={4,0,12,4,4,4,14}, ['j'-32]={2,0,6,2,2,18,12},
  ['k'-32]={16,16,18,20,24,20,18}, ['l'-32]={12,4,4,4,4,4,14},
  ['m'-32]={0,0,26,21,21,21,21}, ['n'-32]={0,0,30,17,17,17,17},
  ['o'-32]={0,0,14,17,17,17,14}, ['p'-32]={0,30,17,17,30,16,16},
  ['q'-32]={0,15,17,17,15,1,1}, ['r'-32]={0,0,22,25,16,16,16},
  ['s'-32]={0,0,15,16,14,1,30}, ['t'-32]={8,8,30,8,8,9,6},
  ['u'-32]={0,0,17,17,17,19,13}, ['v'-32]={0,0,17,17,17,10,4},
  ['w'-32]={0,0,17,17,21,21,10}, ['x'-32]={0,0,17,10,4,10,17},
  ['y'-32]={0,17,17,17,15,1,14}, ['z'-32]={0,0,31,2,4,8,31},
  ['{'-32]={2,4,4,8,4,4,2}, ['|'-32]={4,4,4,4,4,4,4},
  ['}'-32]={8,4,4,2,4,4,8}, ['~'-32]={0,0,9,22,0,0,0}
};

#ifdef FREETYPE_SUPPORT
static void
font_cleanup (void)
{
  if (face) FT_Done_Face (face);
  if (library) FT_Done_FreeType (library);
  face = NULL;
  library = NULL;
}
#endif

const char *
ogl_font_configure (const char *mode, const char *filename)
{
  if (strcmp (mode, "bitmap") == 0)
    return filename ? "-fontfile requires -labelfont freetype" : NULL;
  if (strcmp (mode, "freetype") != 0)
    return "-labelfont must be bitmap or freetype";
#ifdef FREETYPE_SUPPORT
  if (!filename) return "-labelfont freetype requires -fontfile PATH";
  if (FT_Init_FreeType (&library)) return "could not initialize FreeType";
  if (FT_New_Face (library, filename, 0, &face)) {
    font_cleanup();
    return "could not load FreeType font file";
  }
  if (FT_Select_Charmap (face, FT_ENCODING_UNICODE)) {
    font_cleanup();
    return "font has no Unicode character map";
  }
  atexit (font_cleanup);
  return NULL;
#else
  return "FreeType support not built; rebuild with USE_FREETYPE=1";
#endif
}

static void
bitmap_character (int size, int c)
{
  unsigned char pixels[64] = {0};
  int h = size - 2, w = h * 5 / 7, stride = (w + 7) / 8;
  int x, y;
  /* Keep a distinct open ring above the full-height A, even at 10 pixels. */
  if (c == 0xc5 || c == 0x212b) {
    int ring_w = 3, left = (w - ring_w) / 2;
    bitmap_character (size, 'A');
    memset (pixels, 0, sizeof pixels);
    for (y = 0; y < 3; y++)
      for (x = 0; x < ring_w; x++)
        if (y == 1 ? x != 1 : x == 1)
          pixels[y * stride + (left+x)/8] |= 0x80 >> ((left+x)%8);
    glBitmap (w, 3, w+2, -h-1, 0, 0, pixels);
    return;
  }
  if (c == 0xb2) {
    static const unsigned char rows[5] = {6, 9, 2, 4, 15};
    int height = h * 2 / 3, width = w * 2 / 3;
    for (y = 0; y < height; y++)
      for (x = 0; x < width; x++)
        if (rows[4-y*5/height] & (1 << (3-x*4/width)))
          pixels[y * stride + x/8] |= 0x80 >> (x%8);
    glBitmap (w, height, 0, height-h, width+2, 0, pixels);
    return;
  }
  if (c < 32 || c > 126) c = '?';
  for (y = 0; y < h; y++)
    for (x = 0; x < w; x++)
      if (glyphs[c - 32][6 - y * 7 / h] & (1 << (4 - x * 5 / w)))
        pixels[y * stride + x / 8] |= 0x80 >> (x % 8);
  glBitmap (w, h, 0, 0, w + 2, 0, pixels);
}

void
ogl_bitmap_character (void *font, int c)
{
  GLint alignment, lsb, row_length, skip_rows, skip_pixels;
  int size = font == GLUT_BITMAP_HELVETICA_10 ? 10 :
             font == GLUT_BITMAP_HELVETICA_12 ? 12 : 18;
  assert (font);
  glGetIntegerv (GL_UNPACK_ALIGNMENT, &alignment);
  glGetIntegerv (GL_UNPACK_LSB_FIRST, &lsb);
  glGetIntegerv (GL_UNPACK_ROW_LENGTH, &row_length);
  glGetIntegerv (GL_UNPACK_SKIP_ROWS, &skip_rows);
  glGetIntegerv (GL_UNPACK_SKIP_PIXELS, &skip_pixels);
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei (GL_UNPACK_LSB_FIRST, GL_FALSE);
  glPixelStorei (GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei (GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei (GL_UNPACK_SKIP_PIXELS, 0);
#ifdef FREETYPE_SUPPORT
  if (face) {
    FT_GlyphSlot glyph;
    FT_Bitmap *bitmap;
    unsigned char *pixels;
    int y, stride;
    if (FT_Set_Pixel_Sizes (face, 0, size) ||
        FT_Load_Char (face, c, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO)) {
      fprintf (stderr, "Error: could not rasterize FreeType glyph %u\n", (unsigned int) c);
      exit (1);
    }
    glyph = face->glyph;
    bitmap = &glyph->bitmap;
    if (bitmap->width && bitmap->rows && bitmap->pixel_mode != FT_PIXEL_MODE_MONO) {
      fprintf (stderr, "Error: font did not produce a monochrome glyph bitmap\n");
      exit (1);
    }
    stride = (bitmap->width + 7) / 8;
    pixels = calloc (bitmap->rows * stride + 1, 1);
    if (!pixels) { fprintf (stderr, "Error: font bitmap allocation failed\n"); exit (1); }
    for (y = 0; y < (int) bitmap->rows; y++) {
      const unsigned char *row = bitmap->buffer +
        (bitmap->pitch >= 0 ? y * bitmap->pitch : ((int) bitmap->rows - 1 - y) * -bitmap->pitch);
      memcpy (pixels + (bitmap->rows - 1 - y) * stride, row, stride);
    }
    glBitmap (bitmap->width, bitmap->rows, -glyph->bitmap_left,
              (int) bitmap->rows - glyph->bitmap_top,
              glyph->advance.x / 64.0, glyph->advance.y / 64.0, pixels);
    free (pixels);
  } else
#endif
  bitmap_character (size, c);
  glPixelStorei (GL_UNPACK_ALIGNMENT, alignment);
  glPixelStorei (GL_UNPACK_LSB_FIRST, lsb);
  glPixelStorei (GL_UNPACK_ROW_LENGTH, row_length);
  glPixelStorei (GL_UNPACK_SKIP_ROWS, skip_rows);
  glPixelStorei (GL_UNPACK_SKIP_PIXELS, skip_pixels);
}

void
ogl_bitmap_string (void *font, char *str)
{
  const char *text = str;
  assert (font);
  assert (str);
  while (*text) ogl_bitmap_character (font, str_utf8_next (&text));
}
