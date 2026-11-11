#ifndef FIGURE_H
#define FIGURE_H 1

#include "global.h"

#define FIGURE_MAX_TICKS 256
typedef struct {
  char *title;
  double values[7];
  int vertical, custom_ticks, count, categorical;
  double ticks[FIGURE_MAX_TICKS];
  char *labels[FIGURE_MAX_TICKS];
  colour colours[FIGURE_MAX_TICKS];
} figure_key;
extern figure_key legend_config;

int adaptive_segments (int fixed);
void measurement_begin (void);
void measurement_point (void);
void measurement (int angle, char *label, double scale);
void figure_key_begin (char *title, int categorical);
void figure_key_item (char *label);
void figure_key_finish (void);
void figure_swatch (double x, double y, double z, double width, double height, colour *c);

#endif
