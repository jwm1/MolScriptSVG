/* svg.h

   MolScriptSVG v2.1.5

   SVG file.
*/

#ifndef SVG_H
#define SVG_H 1

#include "coord.h"

void svg_set (void);

void svg_first_plot (void);
void svg_finish_output (void);
void svg_start_plot (void);
void svg_finish_plot (void);

void svg_set_background (void);
void svg_set_area (void);

void svg_coil (void);
void svg_cylinder (vector3 *v1, vector3 *v2);
void svg_helix (void);
void svg_label (vector3 *p, char *label, colour *c);
void svg_line (boolean polylines);
void svg_stick (vector3 *v1, vector3 *v2, double r1, double r2, colour *c);
void svg_sphere (at3d *at, double radius);
void svg_strand (void);

void svg_object (int code, vector3 *triplets, int count);

#endif
