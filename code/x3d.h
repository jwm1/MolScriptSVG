/* x3d.h

   MolScript v2.1.2

   X3D Classic.

   Copyright (C) 1997-1998 Per Kraulis
     4-Dec-1996  first attempts
    31-Mar-1997  fairly finished
    18-Aug-1997  split out and generalized segment handling
    15-Oct-1997  USE/DEF for helix and strand different surfaces
*/

#ifndef MOLSCRIPT_X3D_H
#define MOLSCRIPT_X3D_H 1

#include "col.h"
#include "coord.h"

void x3d_set_xml_encoding (boolean xml_encoding);
void x3d_set_webgl_output (boolean webgl_output);
void x3d_set (void);

void x3d_first_plot (void);
void x3d_finish_output (void);
void x3d_start_plot (void);
void x3d_finish_plot (void);

void x3d_set_area (void);
void x3d_set_background (void);
void x3d_anchor_start (char *str);
void x3d_anchor_description (char *str);
void x3d_anchor_parameter (char *str);
void x3d_anchor_start_geometry (void);
void x3d_anchor_finish (void);
void x3d_lod_start (void);
void x3d_lod_finish (void);
void x3d_lod_start_group (void);
void x3d_lod_finish_group (void);
void x3d_viewpoint_start (char *str);
void x3d_viewpoint_output (void);
void x3d_ms_directionallight (void);
void x3d_ms_pointlight (void);
void x3d_ms_spotlight (void);

void x3d_coil (void);
void x3d_cylinder (vector3 *v1, vector3 *v2);
void x3d_helix (void);
void x3d_label (vector3 *p, char *label, colour *c);
void x3d_line (boolean polylines);
void x3d_points (int colours);
void x3d_sphere (at3d *at, double radius);
void x3d_stick (vector3 *v1, vector3 *v2, double r1, double r2, colour *c);
void x3d_strand (void);

void x3d_object (int code, vector3 *triplets, int count);

#endif
