/* xform.c

   MolScriptSVG v2.3

   Coordinate transformation.

   Copyright (C) 1997-1998 Per Kraulis
     6-Apr-1997  split out of coord.c
    23-Jun-1997  added axis rotation
*/

#include <assert.h>

#include "clib/angle.h"
#include "clib/matrix3.h"
#include "clib/quaternion.h"

#include "xform.h"
#include "global.h"
#include "select.h"
#include "graphics.h"
#include "clib/extent3d.h"
#include <math.h>

char *view_filename = NULL;
static double view_matrix[4][4];
static int view_valid, view_transformed, view_drawn, view_plots;

void
view_plot_init (void)
{
  matrix3_initialize (view_matrix);
  view_valid = TRUE; view_transformed = view_drawn = FALSE;
  if (view_filename && ++view_plots > 1) yyerror ("-viewout requires a single plot");
}

void
view_note_read (void)
{
  if (view_transformed) view_valid = FALSE;
}

void
view_note_draw (void)
{
  view_drawn = TRUE;
}

void
view_export (void)
{
  FILE *file;
  int i, j;
  vector3 low, high;
  if (!view_filename) return;
  if (!view_valid) {
    yyerror ("cannot export one view after partial transforms, drawing before transforms, or reading coordinates after transforms");
    return;
  }
  ext3d_get_low_high (&low, &high);
  if (low.x > high.x) { yyerror("cannot export a view of an empty plot"); return; }
  set_extent();
  for (i = 0; i < 4; i++) for (j = 0; j < 4; j++)
    if (!isfinite(view_matrix[i][j])) { yyerror("non-finite view matrix"); return; }
  file = fopen (view_filename, "wx");
  if (!file) { yyerror ("cannot create view file (existing files are not overwritten)"); return; }
  fprintf (file, "! Reusable MolScript view; apply to coordinates in the original frame.\n");
  fprintf (file, "macro view_header\n  area %.17g %.17g %.17g %.17g;\n  window %.17g;\n  slab %.17g;\nend_macro\n",
           area[0], area[1], area[2], area[3], 2*window, 2*slab);
  fprintf (file, "macro view_transform\n  by rotation\n");
  for (i = 0; i < 3; i++)
    fprintf (file, "    %.17g %.17g %.17g\n", view_matrix[0][i], view_matrix[1][i], view_matrix[2][i]);
  fprintf (file, "  by translation %.17g %.17g %.17g\nend_macro\n",
           view_matrix[3][0], view_matrix[3][1], view_matrix[3][2]);
  if (ferror(file)) { fclose(file); yyerror("could not write view file"); return; }
  if (fclose(file)) yyerror("could not close view file");
}


/*============================================================*/
double xform[4][4];

static double tmp_xform[4][4];
static double stored_xform[4][4];


/*------------------------------------------------------------*/
void
xform_init (void)
{
  matrix3_initialize (xform);
}


/*------------------------------------------------------------*/
void
xform_init_stored (void)
{
  matrix3_initialize (stored_xform);
}


/*------------------------------------------------------------*/
void
xform_store (void)
{
  matrix3_copy (stored_xform, xform);
}


/*------------------------------------------------------------*/
void
xform_centre (void)
{
  assert (dstack_size == 3);

  matrix3_translation (tmp_xform, - dstack[0], - dstack[1], - dstack[2]);
  matrix3_concatenate (xform, tmp_xform);

  clear_dstack();
}


/*------------------------------------------------------------*/
void
xform_translation (void)
{
  assert (dstack_size == 3);

  matrix3_translation (tmp_xform, dstack[0], dstack[1], dstack[2]);
  matrix3_concatenate (xform, tmp_xform);

  clear_dstack();
}


/*------------------------------------------------------------*/
void
xform_rotation_x (void)
{
  assert (dstack_size == 1);

  matrix3_x_rotation (tmp_xform, to_radians (dstack[0]));
  matrix3_concatenate (xform, tmp_xform);

  clear_dstack();
}


/*------------------------------------------------------------*/
void
xform_rotation_y (void)
{
  assert (dstack_size == 1);

  matrix3_y_rotation (tmp_xform, to_radians (dstack[0]));
  matrix3_concatenate (xform, tmp_xform);

  clear_dstack();
}


/*------------------------------------------------------------*/
void
xform_rotation_z (void)
{
  assert (dstack_size == 1);

  matrix3_z_rotation (tmp_xform, to_radians (dstack[0]));
  matrix3_concatenate (xform, tmp_xform);

  clear_dstack();
}


/*------------------------------------------------------------*/
void
xform_rotation_axis (void)
{
  vector3 v;
  quaternion q;

  assert (dstack_size == 4);

  v3_initialize (&v, dstack[0], dstack[1], dstack[2]);
  if (v3_length (&v) <= 1.0e-10) {
    yyerror ("invalid rotation axis: zero length");
    return;
  }
  quat_initialize_v3 (&q, &v, to_radians (dstack[3]));
  quat_to_matrix3 (tmp_xform, &q);
  matrix3_concatenate (xform, tmp_xform);

  clear_dstack();
}


/*------------------------------------------------------------*/
void
xform_rotation_matrix (void)
{
  assert (dstack_size == 9);

  matrix3_initialize (tmp_xform);
  tmp_xform[0][0] = dstack[0];
  tmp_xform[1][0] = dstack[1];
  tmp_xform[2][0] = dstack[2];
  tmp_xform[0][1] = dstack[3];
  tmp_xform[1][1] = dstack[4];
  tmp_xform[2][1] = dstack[5];
  tmp_xform[0][2] = dstack[6];
  tmp_xform[1][2] = dstack[7];
  tmp_xform[2][2] = dstack[8];
  matrix3_concatenate (xform, tmp_xform);

  clear_dstack();
}


/*------------------------------------------------------------*/
void
xform_recall_matrix (void)
{
  matrix3_concatenate (xform, stored_xform);
}


/*------------------------------------------------------------*/
void
xform_atoms (void)
{
  mol3d *mol;
  res3d *res;
  at3d *at;
  int *flags;
  int count = 0;
  named_data *nd;

  assert (count_atom_selections() == 1);

  flags = current_atom_sel->flags;
  for (mol = first_molecule; mol; mol = mol->next) {
    for (res = mol->first; res; res = res->next) {
      for (at = res->first; at; at = at->next) {
	if (*flags++) {
	  matrix3_transform (&(at->xyz), xform);
	  count++;
	}
      }
    }
  }

  pop_atom_selection();
  if (count) {
    if (count != total_atoms || view_drawn) view_valid = FALSE;
    matrix3_concatenate (view_matrix, xform);
    view_transformed = TRUE;
  }
  if (check_mode && !count) yywarning ("empty atom selection for transform");

  if (message_mode) {
    int i, j;
    fprintf (stderr, "%i atoms selected for transform\n", count);
    fprintf (stderr, "rotation matrix applied:\n");
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++)
	fprintf (stderr, "%.8f ", xform[j][i]);
      fprintf (stderr, "\n");
    }
    fprintf (stderr, "translation vector applied:\n");
    for (i = 0; i < 3; i++)
      fprintf (stderr, "%.4f ", xform[3][i]);
    fprintf (stderr, "\n");
  }

  assert (count_atom_selections() == 0);
}
