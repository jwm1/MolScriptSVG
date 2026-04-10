/* povray.h

   MolScriptSVG v2.1.5

   POV-Ray scene file output.
*/

#ifndef POVRAY_H
#define POVRAY_H 1

#include "coord.h"

void pov_set (void);

void pov_first_plot (void);
void pov_finish_output (void);
void pov_start_plot (void);
void pov_finish_plot (void);

void pov_set_area (void);
void pov_set_background (void);
void pov_directionallight (void);
void pov_pointlight (void);
void pov_comment (char *str);

void pov_coil (void);
void pov_cylinder (vector3 *v1, vector3 *v2);
void pov_helix (void);
void pov_label (vector3 *p, char *label, colour *c);
void pov_line (boolean polylines);
void pov_sphere (at3d *at, double radius);
void pov_stick (vector3 *v1, vector3 *v2, double r1, double r2, colour *c);
void pov_strand (void);

void pov_object (int code, vector3 *triplets, int count);

#endif
