/* metapost.h

   MolScript v2.1.5

   MetaPost file.
*/

#ifndef METAPOST_H
#define METAPOST_H 1

#include "coord.h"

void mp_set (void);
void mp_set_axes (int enabled);

void mp_first_plot (void);
void mp_finish_output (void);
void mp_start_plot (void);
void mp_finish_plot (void);

void mp_set_background (void);
void mp_set_area (void);

void mp_coil (void);
void mp_cylinder (vector3 *v1, vector3 *v2);
void mp_helix (void);
void mp_label (vector3 *p, char *label, colour *c);
void mp_line (boolean polylines);
void mp_stick (vector3 *v1, vector3 *v2, double r1, double r2, colour *c);
void mp_sphere (at3d *at, double radius);
void mp_strand (void);

void mp_object (int code, vector3 *triplets, int count);

#endif
