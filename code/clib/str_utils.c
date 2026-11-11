/* str_utils

   String utility procedures.

   clib v1.1

   Copyright (C) 1997-1998 Per Kraulis
    13-Feb-1997  first version
    10-May-1997  added hash code function and proper code for str_eq, str_eqn
    23-Jul-1997  added str_eq_min
*/

#include "str_utils.h"

unsigned int
str_utf8_next (const char **text)
{
  const unsigned char *s = (const unsigned char *) *text;
  unsigned int c, minimum;
  int n, i;
  if (!*s) return 0;
  *text += 1;
  if (*s < 0x80) return *s;
  if (*s >= 0xc2 && *s <= 0xdf) { n = 2; c = *s & 31; minimum = 0x80; }
  else if (*s >= 0xe0 && *s <= 0xef) { n = 3; c = *s & 15; minimum = 0x800; }
  else if (*s >= 0xf0 && *s <= 0xf4) { n = 4; c = *s & 7; minimum = 0x10000; }
  else return '?';
  for (i = 1; i < n; i++) {
    if ((s[i] & 0xc0) != 0x80) return '?';
    c = (c << 6) | (s[i] & 63);
  }
  if (c < minimum || c > 0x10ffff || (c >= 0xd800 && c <= 0xdfff)) return '?';
  *text += n - 1;
  return c;
}

/* public ==========
#include <boolean.h>
========== public */

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/*------------------------------------------------------------*/
boolean
str_eq (const char *str1, const char *str2)
     /*
       Are the strings equal? Requires equal length.
     */
{
  /* pre */
  assert (str1);
  assert (str2);

  while (*str1) if (*str1++ != *str2++) return FALSE;
  return (*str2 == '\0');	/* second string must be at end if first is */
}


/*------------------------------------------------------------*/
boolean
str_eqn (const char *str1, const char *str2, const int n)
     /*
       Are the strings equal up to the given length?
     */
{
  register int slot;

  /* pre */
  assert (str1);
  assert (str2);
  assert (n >= 0);

  for (slot = 0; slot < n; slot++) {
    if (*str1 == '\0') return (*str2 == '\0');
    if (*str1++ != *str2++) return FALSE;
  }

  return TRUE;
}


/*------------------------------------------------------------*/
boolean
str_eq_min  (const char *str1, const char *str2)
     /*
       Are the strings equal up to the length of the shortest?
     */
{
  /* pre */
  assert (str1);
  assert (str2);

  while (*str1 && *str2) if (*str1++ != *str2++) return FALSE;
  return TRUE;
}


/*------------------------------------------------------------*/
void 
str_fill_blanks (char *str, int const length)
{
  /* pre */
  assert (str);
  assert (length >= 0);

  memset (str, ' ', length);
  str[length] = '\0';
}


/*------------------------------------------------------------*/
void
str_remove_blanks (char *str)
     /*
       Remove all blank characters from the string, shortening it.
     */
{
  register char *p;

  /* pre */
  assert (str);

  for (p = str; *p; p++) if (*p != ' ') *str++ = *p;
  *str = '\0';
}


/*------------------------------------------------------------*/
void
str_remove_nulls (char *str, int const length)
     /*
       Remove null characters in the string up to the given length,
       putting a null character at that point in the string.
     */
{
  register char *p;
  register int slot;

  /* pre */
  assert (str);
  assert (length >= 0);

  p = str;
  for (slot = 0; slot < length; slot++) {
    if (*p != '\0') *str++ = *p;
    p++;
  }
  *str = '\0';
}


/*------------------------------------------------------------*/
void
str_left_adjust (char *str)
     /*
       Remove any leading white-space from the string.
     */
{
  register char *nb;

  /* pre */
  assert (str);

  nb = str;
  while (*nb && isspace (*nb)) nb++;
  memmove (str, nb, strlen (str) - (str - nb) + 1);
}


/*------------------------------------------------------------*/
void
str_right_adjust (char *str)
     /*
       Remove any trailing white-space from the string.
     */
{
  register int len;

  /* pre */
  assert (str);

  len = strlen (str);
  while (len--) {
    if (! isspace (str[len])) {
      str[len+1] = '\0';
      return;
    }
  }
}


/*------------------------------------------------------------*/
void
str_exchange (char *str, char old, char new)
{
  /* pre */
  assert (str);

  for (; *str; str++) if (*str == old) *str = new;
}


/*------------------------------------------------------------*/
void
str_upcase (char *str)
{
  /* pre */
  assert (str);

  for (; *str; str++) *str = toupper (*str);
}


/*------------------------------------------------------------*/
void
str_lowcase (char *str)
{
  /* pre */
  assert (str);

  for (; *str; str++) *str = tolower (*str);
}


/*------------------------------------------------------------*/
char *
str_alloc (const int length)
     /*
       Allocate space for a string of the given length.
       It is not initialized.
     */
{
  /* pre */
  assert (length > 0);

  return malloc ((length + 1) * sizeof (char));
}


/*------------------------------------------------------------*/
char *
str_clone (const char *str)
     /*
       Create a new string of the same length and with the same
       contents as the given string.
     */
{
  register char *new;
  register int len;

  /* pre */
  assert (str);

  len = strlen (str) + 1;
  new = malloc (len * sizeof (char));
  if (new) {
    return memcpy (new, str, len);
  } else {
    return NULL;
  }
}


/*------------------------------------------------------------*/
char *
str_subclone (const char *str, const int first, const int last)
     /*
       Create a new string containing a substring of the given string.
     */
{
  register char *new;
  register int len = last - first + 1;

  /* pre */
  assert (str);
  assert (first >= 0);
  assert (last >= 0);
  assert (first <= last);
  assert (last <= strlen (str));

  new = malloc ((len + 1) * sizeof (char));
  if (new) {
    memcpy (new, str + first, len);
    new[len] = '\0';
  }
  return new;
}


/*------------------------------------------------------------*/
int
str_hashcode (const char *str, const int range)
     /*
       Compute a hash code for the given string, lower than the given range.
     */
{
  register unsigned int h = 0;
  register unsigned int g;

  /* pre */
  assert (str);
  assert (*str);
  assert (range > 0);

  while (*str) {
    h = (h << 4) + (*str++);
    if (g = h & 0xf0000000) {
      h = h ^ (g >> 24);
      h = h ^ g;
    }
  }

  return h % range;
}
