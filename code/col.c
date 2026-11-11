/* col.c

   MolScriptSVG v2.3

   Colour routines.

   Copyright (C) 1997-1998 Per Kraulis
     4-Dec-1996  first attempts
    29-Jan-1997  use clib colour
*/

#include <assert.h>
#include <string.h>

#include "clib/colour.h"

#include "col.h"
#include "global.h"
#include "lex.h"
#include "state.h"


/*------------------------------------------------------------*/
colour given_colour;
colour white_colour = { COLOUR_GREY, 1.0, 0.0, 0.0 };
colour black_colour = { COLOUR_GREY, 0.0, 0.0, 0.0 };
colour grey_colour  = { COLOUR_GREY, 0.5, 0.0, 0.0 };
colour grey02_colour  = { COLOUR_GREY, 0.2, 0.0, 0.0 };
colour red_colour   = { COLOUR_RGB,  1.0, 0.0, 0.0 };
colour blue_colour  = { COLOUR_RGB,  0.0, 0.0, 1.0 };
colour ramp_from_colour, ramp_to_colour;


/*------------------------------------------------------------*/
void
constant_colours_to_rgb (void)
{
  colour_to_rgb (&white_colour);
  colour_to_rgb (&black_colour);
  colour_to_rgb (&grey_colour);
  colour_to_rgb (&grey02_colour);
}


/*------------------------------------------------------------*/
int
invalid_colour (colour *c)
{
  assert (c);

  if (c->x < 0.0 || c->x > 1.0) {
    if (c->spec == COLOUR_GREY) {
      yyerror ("invalid grey colour value");
      return TRUE;
    } else {
      yyerror ("invalid first component of colour value");
      return TRUE;
    }
  }

  if (c->y < 0.0 || c->y > 1.0) {
    yyerror ("invalid second component of colour value");
    return TRUE;
  }

  if (c->z < 0.0 || c->z > 1.0)  {
    yyerror ("invalid third component of colour value");
    return TRUE;
  }

  return FALSE;
}


/*------------------------------------------------------------*/
void
set_rgb (void)
{
  assert (dstack_size >= 3);

  colour_set_rgb (&given_colour,
		  dstack [dstack_size - 3],
		  dstack [dstack_size - 2],
		  dstack [dstack_size - 1]);
  pop_dstack (3);

  if (invalid_colour (&given_colour)) return;

  assert (colour_valid_state (&given_colour));
}


/*------------------------------------------------------------*/
void
set_hsb (void)
{
  assert (dstack_size >= 3);

  colour_set_hsb (&given_colour,
		  dstack [dstack_size - 3],
		  dstack [dstack_size - 2],
		  dstack [dstack_size - 1]);
  pop_dstack (3);

  if (invalid_colour (&given_colour)) return;

  assert (colour_valid_state (&given_colour));
}


/*------------------------------------------------------------*/
void
set_grey (void)
{
  assert (dstack_size >= 1);

  colour_set_grey (&given_colour, dstack [dstack_size - 1]);
  pop_dstack (1);

  if (invalid_colour (&given_colour)) return;

  assert (colour_valid_state (&given_colour));
}


/*------------------------------------------------------------*/
void
set_colour (const char *name)
{
  assert (name);

  if (! colour_set_name (&given_colour, name)) {
    yyerror ("unknown colour name");
    return;
  }

  assert (colour_valid_state (&given_colour));
}


/*------------------------------------------------------------*/
static double hsb_ramp_length;
static int named_ramp;

int
ramp_is_discrete (void)
{
  return named_ramp == 2;
}

void
set_named_ramp (const char *name)
{
  if (!strcmp (name, "blue-white-red")) named_ramp = 1;
  else if (!strcmp (name, "alphafold")) named_ramp = 2;
  else if (!strcmp (name, "alphafold-gradient")) named_ramp = 3;
  else { yyerror ("unknown named colour ramp"); return; }
  ramp_colour (&ramp_from_colour, 0.0);
  ramp_colour (&ramp_to_colour, 1.0);
}
static double hsb_ramp_breakpoint;

typedef struct {
  int named, hsb;
  colour from, to;
  double length, breakpoint, low, high;
} ramp_snapshot;
typedef struct {
  int valid;
  double value;
  ramp_snapshot ramp;
} residue_ramp;

static void
snapshot_ramp (ramp_snapshot *s)
{
  memset (s, 0, sizeof(*s));
  s->named = named_ramp; s->hsb = current_state->hsbramp;
  s->from = ramp_from_colour; s->to = ramp_to_colour;
  s->length = hsb_ramp_length; s->breakpoint = hsb_ramp_breakpoint;
}

static void
restore_ramp (ramp_snapshot *s)
{
  named_ramp = s->named; current_state->hsbramp = s->hsb;
  ramp_from_colour = s->from; ramp_to_colour = s->to;
  hsb_ramp_length = s->length; hsb_ramp_breakpoint = s->breakpoint;
}

static int
same_ramp (ramp_snapshot *a, ramp_snapshot *b)
{
  return a->named == b->named && a->hsb == b->hsb &&
    a->low == b->low && a->high == b->high &&
    a->length == b->length && a->breakpoint == b->breakpoint &&
    a->from.spec == b->from.spec && a->from.x == b->from.x &&
    a->from.y == b->from.y && a->from.z == b->from.z &&
    a->to.spec == b->to.spec && a->to.x == b->to.x &&
    a->to.y == b->to.y && a->to.z == b->to.z;
}

void
clear_residue_ramp (res3d *res)
{
  residue_ramp *r = nd_search_data (res->data, "molscript-scalar-ramp");
  if (r) r->valid = FALSE;
}

void
record_residue_ramp (res3d *res, double f, double low, double high)
{
  residue_ramp r;
  named_data *nd = nd_search (res->data, "molscript-scalar-ramp");
  memset (&r, 0, sizeof(r));
  r.valid = !ramp_is_discrete();
  r.value = f < 0 ? 0 : f > 1 ? 1 : f;
  snapshot_ramp (&r.ramp);
  r.ramp.low = low; r.ramp.high = high;
  if (!nd) {
    nd = nd_create ("molscript-scalar-ramp", TRUE);
    if (res->data) nd_append (res->data, nd);
    else res->data = nd;
  }
  nd_set_data_clone (nd, &r, sizeof(r));
}

int
interpolate_residue_colour (colour *dest, res3d *a, res3d *b, double t)
{
  residue_ramp *ra, *rb;
  ramp_snapshot saved;
  if (!current_state->colourparts || !current_state->colourinterpolate) return FALSE;
  if (a->mol != b->mol || a->chain != b->chain || a->next != b) return FALSE;
  ra = nd_search_data (a->data, "molscript-scalar-ramp");
  rb = nd_search_data (b->data, "molscript-scalar-ramp");
  if (!ra || !rb || !ra->valid || !rb->valid ||
      !same_ramp (&ra->ramp, &rb->ramp)) return FALSE;
  snapshot_ramp (&saved);
  restore_ramp (&ra->ramp);
  ramp_colour (dest, (1-t)*ra->value + t*rb->value);
  restore_ramp (&saved);
  return TRUE;
}


/*------------------------------------------------------------*/
void
set_colour_ramp (colour *ramp_to)
{
  assert (ramp_to);

  named_ramp = 0;

  ramp_to_colour = *ramp_to;

  if (current_state->hsbramp) {

    colour_to_hsb (&ramp_from_colour);
    colour_to_hsb (&ramp_to_colour);

    if (current_state->hsbrampreverse) {
      if (ramp_from_colour.x < ramp_to_colour.x) {
	hsb_ramp_length = - (ramp_from_colour.x + (1.0 - ramp_to_colour.x));
	hsb_ramp_breakpoint = ramp_from_colour.x / (- hsb_ramp_length);
      } else {
	hsb_ramp_length = ramp_to_colour.x - ramp_from_colour.x;
	hsb_ramp_breakpoint = 1.0;
      }

    } else {
      if (ramp_from_colour.x > ramp_to_colour.x) {
	hsb_ramp_length = (1.0 - ramp_from_colour.x) + ramp_to_colour.x;
	hsb_ramp_breakpoint = (1.0 - ramp_from_colour.x) / hsb_ramp_length;
      } else {
	hsb_ramp_length = ramp_to_colour.x - ramp_from_colour.x;
	hsb_ramp_breakpoint = 1.0;
      }
    }

  } else {
    colour_to_rgb (&ramp_from_colour);
    colour_to_rgb (&ramp_to_colour);
  }

  assert (colour_valid_state (&ramp_from_colour));
  assert (colour_valid_state (&ramp_to_colour));
}


/*------------------------------------------------------------*/
void
set_rainbow_ramp (void)
{
  colour tmp_red_colour = { COLOUR_RGB, 1.0, 0.0, 0.0 };

  current_state->hsbramp = TRUE;
  current_state->hsbrampreverse = TRUE;

  ramp_from_colour = blue_colour;
  set_colour_ramp (&tmp_red_colour);
}


/*------------------------------------------------------------*/
void
ramp_colour (colour *dest, double f)
{
  assert (dest);
  assert (f >= 0.0);
  assert (f <= 1.0);

  if (named_ramp) {
    static const double af[4][3] = {{238,132,83}, {249,220,77},
                                    {117,199,244}, {33,82,206}};
    static const double stops[4] = {0, 0.5, 0.7, 1};
    double rgb[3], t;
    int i, k;
    if (named_ramp == 1) {
      rgb[0] = f < 0.5 ? 2*f : 1;
      rgb[1] = f < 0.5 ? 2*f : 2*(1-f);
      rgb[2] = f < 0.5 ? 1 : 2*(1-f);
    } else {
      i = named_ramp == 2 ? (f > 0.9 ? 3 : f > 0.7 ? 2 : f > 0.5 ? 1 : 0)
                          : (f < 0.5 ? 0 : f < 0.7 ? 1 : 2);
      t = named_ramp == 2 ? 0 : (f-stops[i])/(stops[i+1]-stops[i]);
      for (k=0; k<3; k++)
        rgb[k] = (af[i][k] + (t ? t*(af[i+1][k]-af[i][k]) : 0))/255.0;
    }
    colour_set_rgb (dest, rgb[0], rgb[1], rgb[2]);
    return;
  }

  if (current_state->hsbramp) {
    dest->spec = COLOUR_HSB;
    if (f <= hsb_ramp_breakpoint) {
      dest->x = ramp_from_colour.x + f * hsb_ramp_length;
    } else if (hsb_ramp_length < 0.0) {
      dest->x = 1.0 + (f - hsb_ramp_breakpoint) * hsb_ramp_length;
    } else {
      dest->x = (f - hsb_ramp_breakpoint) * hsb_ramp_length;
    }
    dest->y = ramp_from_colour.y + f * (ramp_to_colour.y - ramp_from_colour.y);
    dest->z = ramp_from_colour.z + f * (ramp_to_colour.z - ramp_from_colour.z);

  } else {
    dest->spec = COLOUR_RGB;
    dest->x = ramp_from_colour.x + f * (ramp_to_colour.x - ramp_from_colour.x);
    dest->y = ramp_from_colour.y + f * (ramp_to_colour.y - ramp_from_colour.y);
    dest->z = ramp_from_colour.z + f * (ramp_to_colour.z - ramp_from_colour.z);
  }

  assert (colour_valid_state (dest));
}
