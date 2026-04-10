/* povray.c

   MolScriptSVG v2.1.5

   POV-Ray scene file output.
*/

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "povray.h"
#include "col.h"
#include "global.h"
#include "graphics.h"
#include "segment.h"
#include "state.h"

#define HELIX_RECESS 0.8
#define POV_LINEWIDTH_FACTOR 0.04
#define POV_LINEWIDTH_MINIMUM 0.005

static colour rgb = { COLOUR_RGB, 0.0, 0.0, 0.0 };
static double pov_camera_distance;
static double pov_camera_right;
static double pov_camera_up;
static vector3 pov_camera_look_at;
static FILE *pov_proper_outfile = NULL;
static FILE *pov_body = NULL;
static double pov_min_x, pov_max_x, pov_min_y, pov_max_y, pov_min_z, pov_max_z;
static int pov_bbox_valid = FALSE;


/*------------------------------------------------------------*/
static void
pov_close_body (void)
{
  if (pov_body) {
    fclose (pov_body);
    pov_body = NULL;
  }
}


/*------------------------------------------------------------*/
static void
pov_reset_bbox (void)
{
  pov_min_x = pov_min_y = pov_min_z = 0.0;
  pov_max_x = pov_max_y = pov_max_z = 0.0;
  pov_bbox_valid = FALSE;
}


/*------------------------------------------------------------*/
static void
pov_include_point_radius (vector3 *v, double radius)
{
  assert (v);

  if (!pov_bbox_valid) {
    pov_min_x = v->x - radius;
    pov_max_x = v->x + radius;
    pov_min_y = v->y - radius;
    pov_max_y = v->y + radius;
    pov_min_z = v->z - radius;
    pov_max_z = v->z + radius;
    pov_bbox_valid = TRUE;
    return;
  }

  if (v->x - radius < pov_min_x) pov_min_x = v->x - radius;
  if (v->x + radius > pov_max_x) pov_max_x = v->x + radius;
  if (v->y - radius < pov_min_y) pov_min_y = v->y - radius;
  if (v->y + radius > pov_max_y) pov_max_y = v->y + radius;
  if (v->z - radius < pov_min_z) pov_min_z = v->z - radius;
  if (v->z + radius > pov_max_z) pov_max_z = v->z + radius;
}


/*------------------------------------------------------------*/
static void
convert_colour (colour *c)
{
  assert (c);
  colour_copy_to_rgb (&rgb, c);
}


/*------------------------------------------------------------*/
static void
pov_v3 (vector3 *v)
{
  assert (v);
  fprintf (outfile, "<%.6f, %.6f, %.6f>", v->x, v->y, v->z);
}


/*------------------------------------------------------------*/
static void
pov_write_finish (void)
{
  double ambient = current_state->lightambientintensity;
  double phong = current_state->shininess;
  double phong_size = 10.0 + 110.0 * phong;

  if (ambient < 0.0) ambient = 0.0;
  if (ambient > 1.0) ambient = 1.0;
  if (ambient < 0.12) ambient = 0.12;

  fprintf (outfile,
           "finish { ambient %.4f diffuse 0.85 phong %.4f phong_size %.2f }\n",
           ambient, phong, phong_size);
}


/*------------------------------------------------------------*/
static void
pov_write_texture (colour *c)
{
  assert (c);

  convert_colour (c);
  fprintf (outfile,
           "texture {\n"
           "  pigment { color rgbt <%.6f, %.6f, %.6f, %.6f> }\n",
           rgb.x, rgb.y, rgb.z, current_state->transparency);
  fputs ("  ", outfile);
  pov_write_finish();
  fputs ("}\n", outfile);
}


/*------------------------------------------------------------*/
static void
pov_write_triangle (vector3 *p1, vector3 *p2, vector3 *p3, colour *c)
{
  pov_include_point_radius (p1, 0.0);
  pov_include_point_radius (p2, 0.0);
  pov_include_point_radius (p3, 0.0);
  fputs ("triangle {\n  ", outfile);
  pov_v3 (p1); fputs (", ", outfile); pov_v3 (p2); fputs (", ", outfile); pov_v3 (p3);
  fputs ("\n  ", outfile);
  pov_write_texture (c);
  fputs ("}\n", outfile);
}


/*------------------------------------------------------------*/
static void
pov_write_smooth_triangle (vector3 *p1, vector3 *n1,
                           vector3 *p2, vector3 *n2,
                           vector3 *p3, vector3 *n3,
                           colour *c)
{
  pov_include_point_radius (p1, 0.0);
  pov_include_point_radius (p2, 0.0);
  pov_include_point_radius (p3, 0.0);
  fputs ("smooth_triangle {\n  ", outfile);
  pov_v3 (p1); fputs (", ", outfile); pov_v3 (n1); fputs (",\n  ", outfile);
  pov_v3 (p2); fputs (", ", outfile); pov_v3 (n2); fputs (",\n  ", outfile);
  pov_v3 (p3); fputs (", ", outfile); pov_v3 (n3); fputs ("\n  ", outfile);
  pov_write_texture (c);
  fputs ("}\n", outfile);
}


/*------------------------------------------------------------*/
static void
pov_write_sphere (vector3 *p, double radius, colour *c)
{
  pov_include_point_radius (p, radius);
  fputs ("sphere {\n  ", outfile);
  pov_v3 (p);
  fprintf (outfile, ", %.6f\n  ", radius);
  pov_write_texture (c);
  fputs ("}\n", outfile);
}


/*------------------------------------------------------------*/
static void
pov_write_cylinder (vector3 *p1, vector3 *p2, double radius, colour *c)
{
  pov_include_point_radius (p1, radius);
  pov_include_point_radius (p2, radius);
  fputs ("cylinder {\n  ", outfile);
  pov_v3 (p1); fputs (", ", outfile); pov_v3 (p2);
  fprintf (outfile, ", %.6f\n  ", radius);
  pov_write_texture (c);
  fputs ("}\n", outfile);
}


/*------------------------------------------------------------*/
static void
pov_write_line_segment (vector3 *p1, vector3 *p2, colour *c)
{
  int slot, parts;
  vector3 pos1, pos2;
  double radius = POV_LINEWIDTH_FACTOR * current_state->linewidth;

  if (radius < POV_LINEWIDTH_MINIMUM) radius = POV_LINEWIDTH_MINIMUM;

  if (current_state->linedash == 0.0) {
    pov_write_cylinder (p1, p2, radius, c);
  } else {
    parts = (int) (v3_distance (p1, p2) * 16.0 / current_state->linedash);
    if (parts <= 1) {
      pov_write_cylinder (p1, p2, radius, c);
    } else {
      if (parts % 2 == 0) parts++;
      pov_write_sphere (p1, radius, c);
      for (slot = 0; slot < parts; slot += 2) {
        v3_between (&pos1, p1, p2, (double) slot / (double) parts);
        v3_between (&pos2, p1, p2, (double) (slot + 1) / (double) parts);
        pov_write_cylinder (&pos1, &pos2, radius, c);
      }
      pov_write_sphere (p2, radius, c);
    }
  }
}


/*------------------------------------------------------------*/
static void
convert_vector_colour (vector3 *v)
{
  rgb.spec = COLOUR_RGB;
  rgb.x = v->x;
  rgb.y = v->y;
  rgb.z = v->z;
}


/*------------------------------------------------------------*/
void
pov_set (void)
{
  output_first_plot = pov_first_plot;
  output_start_plot = pov_start_plot;
  output_finish_plot = pov_finish_plot;
  output_finish_output = pov_finish_output;

  set_area = pov_set_area;
  set_background = pov_set_background;
  anchor_start = do_nothing_str;
  anchor_description = do_nothing_str;
  anchor_parameter = do_nothing_str;
  anchor_start_geometry = do_nothing;
  anchor_finish = do_nothing;
  lod_start = do_nothing;
  lod_finish = do_nothing;
  lod_start_group = do_nothing;
  lod_finish_group = do_nothing;
  viewpoint_start = do_nothing_str;
  viewpoint_output = do_nothing;
  output_directionallight = pov_directionallight;
  output_pointlight = pov_pointlight;
  output_spotlight = do_nothing;
  output_comment = pov_comment;

  output_coil = pov_coil;
  output_cylinder = pov_cylinder;
  output_helix = pov_helix;
  output_label = pov_label;
  output_line = pov_line;
  output_sphere = pov_sphere;
  output_stick = pov_stick;
  output_strand = pov_strand;

  output_start_object = do_nothing;
  output_object = pov_object;
  output_finish_object = do_nothing;

  output_pickable = NULL;

  constant_colours_to_rgb();
  output_mode = POVRAY_MODE;
}


/*------------------------------------------------------------*/
void
pov_first_plot (void)
{
  if (!first_plot) yyerror ("only one plot per input file allowed for POV-Ray");
  set_outfile ("w");
  pov_proper_outfile = outfile;
  if (tmp_filename == NULL) {
    outfile = tmpfile();
  } else {
    outfile = fopen (tmp_filename, "w+");
  }
  if (outfile == NULL) yyerror ("could not create the temporary POV-Ray stream");
  pov_body = outfile;
}


/*------------------------------------------------------------*/
void
pov_finish_output (void)
{
  int ch;

  assert (pov_body);
  assert (pov_proper_outfile);

  outfile = pov_proper_outfile;
  fprintf (outfile, "// POV-Ray scene generated by %s\n", program_str);
  fprintf (outfile, "// %s\n", copyright_str);
  if (title) fprintf (outfile, "// title: %s\n", title);
  fprintf (outfile,
           "// fitted orthographic camera: distance=%.6f right=%.6f up=%.6f\n",
           pov_camera_distance, pov_camera_right, pov_camera_up);
  fputs ("#version 3.7;\n", outfile);
  fputs ("global_settings { assumed_gamma 1.0 ambient_light rgb <0.25, 0.25, 0.25> }\n",
         outfile);

  convert_colour (&background_colour);
  fprintf (outfile, "background { color rgb <%.6f, %.6f, %.6f> }\n",
           rgb.x, rgb.y, rgb.z);
  fprintf (outfile,
           "camera {\n"
           "  orthographic\n"
           "  location <%.6f, %.6f, %.6f>\n"
           "  look_at <%.6f, %.6f, %.6f>\n"
           "  right <%.6f, 0, 0>\n"
           "  up <0, %.6f, 0>\n"
           "}\n",
           pov_camera_look_at.x, pov_camera_look_at.y,
           pov_camera_look_at.z - pov_camera_distance,
           pov_camera_look_at.x, pov_camera_look_at.y, pov_camera_look_at.z,
           pov_camera_right, pov_camera_up);

  rewind (pov_body);
  while ((ch = fgetc (pov_body)) != EOF) fputc (ch, outfile);

  pov_close_body();
  pov_proper_outfile = NULL;
}


/*------------------------------------------------------------*/
void
pov_start_plot (void)
{
  set_area_values (-0.5, -0.5, 0.5, 0.5);
  background_colour = white_colour;
  pov_camera_distance = 200.0;
  pov_camera_right = 120.0;
  pov_camera_up = 120.0;
  v3_initialize (&pov_camera_look_at, 0.0, 0.0, 0.0);
  pov_reset_bbox();
}


/*------------------------------------------------------------*/
void
pov_finish_plot (void)
{
  double width, height, depth, pad;
  vector3 light_pos, fill1_pos, fill2_pos;

  set_extent();
  if (pov_bbox_valid) {
    pov_camera_look_at.x = 0.5 * (pov_min_x + pov_max_x);
    pov_camera_look_at.y = 0.5 * (pov_min_y + pov_max_y);
    pov_camera_look_at.z = 0.5 * (pov_min_z + pov_max_z);
    width = pov_max_x - pov_min_x;
    height = pov_max_y - pov_min_y;
    depth = pov_max_z - pov_min_z;
    pad = 1.5;
    if (width < 1.0) width = 1.0;
    if (height < 1.0) height = 1.0;
    pov_camera_right = width + 2.0 * pad;
    pov_camera_up = height + 2.0 * pad;
    pov_camera_distance = depth + 8.0;
  } else {
    pov_camera_right = 2.0 * aspect_window_x;
    pov_camera_up = 2.0 * aspect_window_y;
    pov_camera_distance = (slab > 0.0) ? (2.0 * slab + 0.5 * window) : (2.5 * window);
    v3_initialize (&pov_camera_look_at, 0.0, 0.0, 0.0);
  }
  if (pov_camera_distance < 20.0) pov_camera_distance = 20.0;

  light_pos.x = pov_camera_look_at.x - 0.6 * pov_camera_right;
  light_pos.y = pov_camera_look_at.y + 0.6 * pov_camera_up;
  light_pos.z = pov_camera_look_at.z - pov_camera_distance;
  fputs ("light_source { ", outfile);
  pov_v3 (&light_pos);
  fputs (" color rgb <1, 1, 1> }\n", outfile);

  fill1_pos.x = pov_camera_look_at.x + 0.7 * pov_camera_right;
  fill1_pos.y = pov_camera_look_at.y + 0.3 * pov_camera_up;
  fill1_pos.z = pov_camera_look_at.z - 0.8 * pov_camera_distance;
  fputs ("light_source { ", outfile);
  pov_v3 (&fill1_pos);
  fputs (" color rgb <0.60, 0.60, 0.60> shadowless }\n", outfile);

  fill2_pos.x = pov_camera_look_at.x - 0.4 * pov_camera_right;
  fill2_pos.y = pov_camera_look_at.y - 0.8 * pov_camera_up;
  fill2_pos.z = pov_camera_look_at.z - 0.6 * pov_camera_distance;
  fputs ("light_source { ", outfile);
  pov_v3 (&fill2_pos);
  fputs (" color rgb <0.40, 0.40, 0.40> shadowless }\n", outfile);

  if (message_mode) fprintf (stderr, "POV-Ray scene output completed.\n");
}


/*------------------------------------------------------------*/
void
pov_set_area (void)
{
  if (message_mode) fprintf (stderr, "ignoring 'area' for POV-Ray output\n");
  clear_dstack();
}


/*------------------------------------------------------------*/
void
pov_set_background (void)
{
  background_colour = given_colour;
}


/*------------------------------------------------------------*/
void
pov_directionallight (void)
{
  vector3 pos;

  assert ((dstack_size == 3) || (dstack_size == 6));
  if (dstack_size == 3) {
    pos.x = -dstack[0] * pov_camera_distance;
    pos.y = -dstack[1] * pov_camera_distance;
    pos.z = -dstack[2] * pov_camera_distance;
  } else {
    pos.x = dstack[0];
    pos.y = dstack[1];
    pos.z = dstack[2];
  }
  convert_colour (&(current_state->lightcolour));
  fputs ("light_source { ", outfile);
  pov_v3 (&pos);
  fprintf (outfile, " color rgb <%.6f, %.6f, %.6f> parallel point_at <0, 0, 0> }\n",
           rgb.x, rgb.y, rgb.z);
  clear_dstack();
}


/*------------------------------------------------------------*/
void
pov_pointlight (void)
{
  vector3 pos;
  pos.x = dstack[0];
  pos.y = dstack[1];
  pos.z = dstack[2];
  convert_colour (&(current_state->lightcolour));
  fputs ("light_source { ", outfile);
  pov_v3 (&pos);
  fprintf (outfile, " color rgb <%.6f, %.6f, %.6f> }\n",
           rgb.x, rgb.y, rgb.z);
  clear_dstack();
}


/*------------------------------------------------------------*/
void
pov_comment (char *str)
{
  fprintf (outfile, "// %s\n", str);
}


/*------------------------------------------------------------*/
void
pov_coil (void)
{
  int slot;
  coil_segment *cs1, *cs2;
  colour *col;

  for (slot = 1; slot < coil_segment_count; slot++) {
    cs1 = coil_segments + slot - 1;
    cs2 = coil_segments + slot;
    col = current_state->colourparts ? &(cs1->c) : &(current_state->planecolour);
    pov_write_cylinder (&(cs1->p), &(cs2->p), current_state->coilradius, col);
  }
}


/*------------------------------------------------------------*/
void
pov_cylinder (vector3 *v1, vector3 *v2)
{
  pov_write_cylinder (v1, v2, current_state->cylinderradius,
                      &(current_state->planecolour));
}


/*------------------------------------------------------------*/
void
pov_helix (void)
{
  int slot;
  helix_segment *hs1, *hs2;
  vector3 p1, p2, p21, p22;
  double radius = current_state->helixthickness / 2.0;
  double offset;
  colour *col;

  for (slot = 1; slot < helix_segment_count; slot++) {
    hs1 = helix_segments + slot - 1;
    hs2 = hs1 + 1;
    col = current_state->colourparts ? &(hs1->c) : &(current_state->planecolour);
    pov_write_smooth_triangle (&(hs1->p1), &(hs1->n), &(hs1->p2), &(hs1->n),
                               &(hs2->p1), &(hs2->n), col);
    pov_write_smooth_triangle (&(hs1->p2), &(hs1->n), &(hs2->p1), &(hs2->n),
                               &(hs2->p2), &(hs2->n), col);
  }

  if (current_state->helixthickness > 0.01) {
    offset = - HELIX_RECESS * radius;
    for (slot = 1; slot < helix_segment_count; slot++) {
      hs1 = helix_segments + slot - 1;
      hs2 = hs1 + 1;
      col = current_state->colourparts ? &(hs1->c) : &(current_state->plane2colour);
      v3_sum_scaled (&p1, &(hs1->p1), offset, &(hs1->n));
      v3_sum_scaled (&p2, &(hs2->p1), offset, &(hs2->n));
      pov_write_cylinder (&p1, &p2, radius, col);
      v3_sum_scaled (&p1, &(hs1->p2), offset, &(hs1->n));
      v3_sum_scaled (&p2, &(hs2->p2), offset, &(hs2->n));
      pov_write_cylinder (&p1, &p2, radius, col);
    }
  }

  if (current_state->helixthickness > 0.01) {
    offset = (HELIX_RECESS + 1.0) / 2.0 * current_state->helixthickness;
  } else {
    offset = 0.02;
  }

  for (slot = 0; slot < helix_segment_count; slot++) {
    v3_reverse (&(helix_segments + slot)->n);
  }
  for (slot = 1; slot < helix_segment_count; slot++) {
    hs1 = helix_segments + slot - 1;
    hs2 = hs1 + 1;
    col = current_state->colourparts ? &(hs1->c) : &(current_state->plane2colour);
    v3_sum_scaled (&p1, &(hs1->p1), offset, &(hs1->n));
    v3_sum_scaled (&p2, &(hs1->p2), offset, &(hs1->n));
    v3_sum_scaled (&p21, &(hs2->p1), offset, &(hs2->n));
    v3_sum_scaled (&p22, &(hs2->p2), offset, &(hs2->n));
    pov_write_smooth_triangle (&p1, &(hs1->n), &p2, &(hs1->n), &p21, &(hs2->n), col);
    pov_write_smooth_triangle (&p2, &(hs1->n), &p21, &(hs2->n), &p22, &(hs2->n), col);
  }
}


/*------------------------------------------------------------*/
void
pov_label (vector3 *p, char *label, colour *c)
{
  fprintf (outfile, "// label \"%s\" at ", label);
  pov_v3 (p);
  fputc ('\n', outfile);
}


/*------------------------------------------------------------*/
void
pov_line (boolean polylines)
{
  int slot;
  line_segment *ls1, *ls2;
  colour *col;

  if (line_segment_count < 2) return;
  if (polylines) {
    for (slot = 1; slot < line_segment_count; slot++) {
      ls1 = line_segments + slot - 1;
      ls2 = line_segments + slot;
      if (ls2->new) continue;
      col = current_state->colourparts ? &(ls1->c) : &(current_state->linecolour);
      pov_write_line_segment (&(ls1->p), &(ls2->p), col);
    }
  } else {
    for (slot = 0; slot < line_segment_count; slot += 2) {
      ls1 = line_segments + slot;
      ls2 = line_segments + slot + 1;
      col = current_state->colourparts ? &(ls1->c) : &(current_state->linecolour);
      pov_write_line_segment (&(ls1->p), &(ls2->p), col);
    }
  }
}


/*------------------------------------------------------------*/
void
pov_sphere (at3d *at, double radius)
{
  pov_write_sphere (&(at->xyz), radius, &(at->colour));
}


/*------------------------------------------------------------*/
void
pov_stick (vector3 *v1, vector3 *v2, double r1, double r2, colour *c)
{
  pov_write_cylinder (v1, v2, current_state->stickradius,
                      c ? c : &(current_state->planecolour));
}


/*------------------------------------------------------------*/
void
pov_strand (void)
{
  int slot;
  int thickness = current_state->strandthickness >= 0.01;
  strand_segment *ss1, *ss2;
  colour *col;

  for (slot = 1; slot < strand_segment_count - 3; slot++) {
    ss1 = strand_segments + slot - 1;
    ss2 = ss1 + 1;
    col = current_state->colourparts ? &(ss1->c) : &(current_state->planecolour);
    pov_write_smooth_triangle (&(ss1->p1), &(ss1->n1), &(ss1->p4), &(ss1->n1),
                               &(ss2->p1), &(ss2->n1), col);
    pov_write_smooth_triangle (&(ss1->p4), &(ss1->n1), &(ss2->p1), &(ss2->n1),
                               &(ss2->p4), &(ss2->n1), col);
  }

  if (!thickness) return;

  for (slot = 1; slot < strand_segment_count - 3; slot++) {
    ss1 = strand_segments + slot - 1;
    ss2 = ss1 + 1;
    col = current_state->colourparts ? &(ss1->c) : &(current_state->plane2colour);
    pov_write_smooth_triangle (&(ss1->p2), &(ss1->n3), &(ss1->p3), &(ss1->n3),
                               &(ss2->p2), &(ss2->n3), col);
    pov_write_smooth_triangle (&(ss1->p3), &(ss1->n3), &(ss2->p2), &(ss2->n3),
                               &(ss2->p3), &(ss2->n3), col);
    pov_write_triangle (&(ss1->p2), &(ss1->p1), &(ss2->p2), col);
    pov_write_triangle (&(ss1->p1), &(ss2->p2), &(ss2->p1), col);
    pov_write_triangle (&(ss1->p3), &(ss1->p4), &(ss2->p3), col);
    pov_write_triangle (&(ss1->p4), &(ss2->p3), &(ss2->p4), col);
  }
}


/*------------------------------------------------------------*/
void
pov_object (int code, vector3 *triplets, int count)
{
  int slot;
  vector3 *v;
  colour c;
  double radius;

  switch (code) {
  case OBJ_POINTS:
    radius = POV_LINEWIDTH_FACTOR * current_state->linewidth;
    if (radius < POV_LINEWIDTH_MINIMUM) radius = POV_LINEWIDTH_MINIMUM;
    for (slot = 0; slot < count; slot++) {
      pov_write_sphere (triplets + slot, radius, &(current_state->linecolour));
    }
    break;
  case OBJ_POINTS_COLOURS:
    radius = POV_LINEWIDTH_FACTOR * current_state->linewidth;
    if (radius < POV_LINEWIDTH_MINIMUM) radius = POV_LINEWIDTH_MINIMUM;
    for (slot = 0; slot < count; slot += 2) {
      convert_vector_colour (triplets + slot + 1);
      c = rgb;
      pov_write_sphere (triplets + slot, radius, &c);
    }
    break;
  case OBJ_LINES:
    for (slot = 1; slot < count; slot++) {
      pov_write_line_segment (triplets + slot - 1, triplets + slot,
                              &(current_state->linecolour));
    }
    break;
  case OBJ_LINES_COLOURS:
    for (slot = 2; slot < count; slot += 2) {
      v = triplets + slot;
      convert_vector_colour (v - 1);
      c = rgb;
      pov_write_line_segment (v - 2, v, &c);
    }
    break;
  case OBJ_TRIANGLES:
    for (slot = 0; slot < count; slot += 3) {
      v = triplets + slot;
      pov_write_triangle (v, v + 1, v + 2, &(current_state->planecolour));
    }
    break;
  case OBJ_TRIANGLES_COLOURS:
    for (slot = 0; slot < count; slot += 6) {
      v = triplets + slot;
      convert_vector_colour (v + 1);
      c = rgb;
      pov_write_triangle (v, v + 2, v + 4, &c);
    }
    break;
  case OBJ_TRIANGLES_NORMALS:
    for (slot = 0; slot < count; slot += 6) {
      v = triplets + slot;
      pov_write_smooth_triangle (v, v + 1, v + 2, v + 3, v + 4, v + 5,
                                 &(current_state->planecolour));
    }
    break;
  case OBJ_TRIANGLES_NORMALS_COLOURS:
    for (slot = 0; slot < count; slot += 9) {
      v = triplets + slot;
      convert_vector_colour (v + 2);
      c = rgb;
      pov_write_smooth_triangle (v, v + 1, v + 3, v + 4, v + 6, v + 7, &c);
    }
    break;
  case OBJ_STRIP:
    for (slot = 2; slot < count; slot++) {
      v = triplets + slot;
      pov_write_triangle (v - 2, v - 1, v, &(current_state->planecolour));
    }
    break;
  case OBJ_STRIP_COLOURS:
    for (slot = 4; slot < count; slot += 2) {
      v = triplets + slot;
      convert_vector_colour (v + 1);
      c = rgb;
      pov_write_triangle (v - 4, v - 2, v, &c);
    }
    break;
  case OBJ_STRIP_NORMALS:
    for (slot = 4; slot < count; slot += 2) {
      v = triplets + slot;
      pov_write_smooth_triangle (v - 4, v - 3, v - 2, v - 1, v, v + 1,
                                 &(current_state->planecolour));
    }
    break;
  case OBJ_STRIP_NORMALS_COLOURS:
    for (slot = 6; slot < count; slot += 3) {
      v = triplets + slot;
      convert_vector_colour (v + 2);
      c = rgb;
      pov_write_smooth_triangle (v - 6, v - 5, v - 3, v - 2, v, v + 1, &c);
    }
    break;
  default:
    break;
  }
}
