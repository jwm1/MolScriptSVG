/* Explicit figure annotations and sampling controls. */
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "figure.h"
#include "graphics.h"
#include "svg.h"
#include "col.h"
#include "clib/str_utils.h"
#include "clib/extent3d.h"
#include "clib/hermite_curve.h"

figure_key legend_config;
static vector3 measurement_points[3];
static int measurement_point_count;

void measurement_begin (void) { measurement_point_count = 0; }

void
measurement_point (void)
{
  assert (dstack_size == 3 && measurement_point_count < 3);
  v3_initialize (measurement_points + measurement_point_count++, dstack[0], dstack[1], dstack[2]);
  /* Numeric selection predicates for the next point need their own stack. */
  clear_dstack();
}

/* For a cubic, the second derivative is linear. Its endpoint norms bound
   chord error by M/(8*n*n) on each equal parameter interval. */
int
adaptive_segments (int fixed)
{
  vector3 p0, p1, v0, v1, a, b;
  double m, count;
  int n = fixed;
  if (current_state->curvetolerance > 0) {
    hermite_get (&p0, 0); hermite_get (&p1, 1);
    hermite_get_tangent (&v0, 0); hermite_get_tangent (&v1, 1);
    v3_difference (&a, &p1, &p0); v3_scale (&a, 6);
    b = a; v3_scale (&b, -1);
    v3_add_scaled (&a, -4, &v0); v3_add_scaled (&a, -2, &v1);
    v3_add_scaled (&b, 2, &v0); v3_add_scaled (&b, 4, &v1);
    m = fmax (v3_length(&a), v3_length(&b));
    count = ceil (sqrt (m/(8*current_state->curvetolerance)));
    if (!isfinite(count) || count > 4096) {
      yyerror ("curvetolerance requires more than 4096 samples per residue interval");
      return fixed;
    }
    n = count < 2 ? 2 : (int)count;
    if (n % 2) n++; /* Retain exact categorical boundaries at t=0.5. */
  }
  if (current_state->colourparts && current_state->colourinterpolate && n < 16) n = 16;
  return n;
}

static void
annotation_line (vector3 *a, vector3 *b, double dash)
{
  vector3 p[2];
  double length = v3_distance(a, b), t, end;
  int i, n;
  if (!isfinite(length) || length < 1e-10) return;
  ext3d_update (a, 0); ext3d_update (b, 0);
  if (dash == 0) { p[0] = *a; p[1] = *b; output_object(OBJ_LINES, p, 2); return; }
  if (length/dash > 20000) { yyerror("measurement dash spacing produces too many pieces"); return; }
  n = (int)ceil(length/dash);
  for (i = 0; i < n; i += 2) {
    t = (double)i/n; end = (double)(i+1)/n;
    v3_scaled (&p[0], 1-t, a); v3_add_scaled (&p[0], t, b);
    v3_scaled (&p[1], 1-end, a); v3_add_scaled (&p[1], end, b);
    output_object (OBJ_LINES, p, 2);
  }
}

void
measurement (int angle, char *label, double scale)
{
  vector3 *p = measurement_points, u, v, side, pos, a, b;
  double d, lu, lv, dot, theta, radius, t, dash;
  int i, steps;
  char text[256];
  if (angle == 0 && dstack_size == 1) { scale = dstack[0]; pop_dstack(1); }
  assert (dstack_size == 0 && measurement_point_count == (angle == 1 ? 3 : 2));
  clear_dstack();
  if (!isfinite(scale) || scale <= 0) { yyerror("measurement scale must be positive"); return; }
  push_state(); new_state();
  dash = current_state->linedash > 0 ? current_state->linedash*LINEWIDTH_FACTOR : 0.3;
  current_state->linedash = 0;
  output_start_object();
  if (angle == 1) {
    v3_difference (&u, p, p+1); v3_difference (&v, p+2, p+1);
    lu = v3_length (&u); lv = v3_length (&v);
    if (!isfinite(lu) || !isfinite(lv) || lu < 1e-10 || lv < 1e-10) {
      yyerror("angle requires finite, distinct vertex and endpoints"); return;
    }
    v3_scale (&u, 1/lu); v3_scale (&v, 1/lv);
    dot = fmax(-1, fmin(1, v3_dot_product(&u, &v)));
    theta = acos(dot);
    annotation_line (p, p+1, dash); annotation_line (p+1, p+2, dash);
    v3_sum_scaled (&side, &v, -dot, &u);
    if (v3_length(&side) < 1e-10) {
      vector3 axis = fabs(u.x) < .9 ? xaxis : yaxis;
      v3_cross_product (&side, &u, &axis);
    }
    v3_normalize (&side);
    radius = .25*fmin(lu, lv);
    steps = 32;
    v3_sum_scaled (&a, p+1, radius, &u);
    for (i = 1; i <= steps; i++) {
      t = theta*i/steps;
      v3_sum_scaled (&b, p+1, radius*cos(t), &u);
      v3_add_scaled (&b, radius*sin(t), &side);
      current_state->linedash = 0;
      annotation_line (&a, &b, 0); a = b;
    }
    v3_sum_scaled (&pos, p+1, 1.4*radius*cos(theta/2), &u);
    v3_add_scaled (&pos, 1.4*radius*sin(theta/2), &side);
    snprintf (text, sizeof(text), "%.2f deg", theta*180/acos(-1));
  } else {
    d = v3_distance(p, p+1);
    if (!isfinite(d*scale)) { yyerror("non-finite measurement distance"); return; }
    annotation_line (p, p+1, dash);
    v3_middle (&pos, p, p+1);
    if (angle == 2) { pos = p[1]; snprintf(text, sizeof(text), "%s", label); }
    else snprintf (text, sizeof(text), "%.2f %s", d*scale, label ? label : "\303\205");
  }
  output_finish_object();
  output_label (&pos, text, &current_state->linecolour);
  v3_add (&pos, &current_state->labeloffset);
  ext3d_update (&pos, 0);
  pop_state();
}

void
figure_swatch (double x, double y, double z, double width, double height, colour *c)
{
  vector3 vertices[12];
  colour rgb = *c;
  int j;
  colour_to_rgb (&rgb);
  v3_initialize (vertices, x, y, z);
  v3_initialize (vertices+2, x+width, y, z);
  v3_initialize (vertices+4, x+width, y+height, z);
  vertices[6] = vertices[0]; vertices[8] = vertices[4];
  v3_initialize (vertices+10, x, y+height, z);
  for (j = 1; j < 12; j += 2) v3_initialize (vertices+j, rgb.x, rgb.y, rgb.z);
  if (output_mode == SVG_MODE) svg_legend_box (vertices, vertices+4, &rgb);
  else output_object (OBJ_TRIANGLES_COLOURS, vertices, 12);
  for (j = 0; j < 12; j += 2) ext3d_update (vertices+j, 0);
}

void
figure_key_begin (char *title, int categorical)
{
  int i;
  memset (&legend_config, 0, sizeof(legend_config));
  legend_config.title = str_clone (title);
  legend_config.categorical = categorical;
  assert (dstack_size == (categorical ? 5 : 7));
  for (i = 0; i < dstack_size; i++) legend_config.values[i] = dstack[i];
  clear_dstack();
}

void
figure_key_item (char *label)
{
  int i = legend_config.count;
  if (i == FIGURE_MAX_TICKS) { yyerror("too many legend items (maximum 256)"); return; }
  legend_config.labels[i] = str_clone (label);
  if (legend_config.categorical) legend_config.colours[i] = given_colour;
  else {
    assert(dstack_size == 1);
    legend_config.ticks[i] = dstack[0]; clear_dstack();
  }
  legend_config.count++;
}

void
figure_key_finish (void)
{
  int i, n = legend_config.count;
  double *s = legend_config.values, w, h, x, y;
  vector3 p;
  if (!legend_config.categorical) {
    for (i = 0; i < 7; i++) push_double(s[i]);
    legend (legend_config.title);
  } else {
    if (!n || s[3] <= 0 || s[4] <= 0) { yyerror("key requires positive dimensions and at least one item"); return; }
    push_state(); new_state();
    current_state->transparency = current_state->depthcue = 0;
    current_state->labelrotation = FALSE;
    current_state->labelcentre = !legend_config.vertical;
    v3_initialize (&current_state->labeloffset, 0, 0, 0);
    w = legend_config.vertical ? s[3] : s[3]/n;
    h = legend_config.vertical ? s[4]/n : s[4];
    output_start_object();
    for (i = 0; i < n; i++) {
      x = s[0] + (legend_config.vertical ? 0 : i*w);
      y = s[1] + (legend_config.vertical ? (n-1-i)*h : 0);
      figure_swatch(x, y, s[2], .8*w, .8*h, legend_config.colours+i);
    }
    output_finish_object();
    for (i = 0; i < n; i++) {
      x = s[0] + (legend_config.vertical ? 1.2*w : (i+.4)*w);
      y = s[1] + (legend_config.vertical ? (n-1-i+.4)*h : -h);
      v3_initialize (&p, x, y, s[2]);
      output_label (&p, legend_config.labels[i], &current_state->linecolour);
      ext3d_update (&p, 0);
    }
    v3_initialize (&p, s[0], s[1]+s[4]+h, s[2]);
    output_label (&p, legend_config.title, &current_state->linecolour);
    ext3d_update (&p, 0);
    pop_state();
  }
  for (i = 0; i < n; i++) free(legend_config.labels[i]);
  free (legend_config.title);
  memset (&legend_config, 0, sizeof(legend_config));
}
