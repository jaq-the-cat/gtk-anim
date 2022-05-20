#pragma once
#include <gtk-3.0/gtk/gtk.h>
#include "figs.h"
#include "pmath.h"

#define NT_ROOT 0
#define NT_OTHER 1

gdouble point_distance(point p0, point p1);
void draw_line(cairo_t *cr, GdkRGBA *color, point p0, point p1, gdouble width);
void draw_filled(cairo_t *cr, GdkRGBA *color, point p0, point p1, gdouble width);
void draw_white(cairo_t *cr, GdkRGBA *color, point p0, point p1, gdouble width);
void draw_empty(cairo_t *cr, GdkRGBA *color, point p0, point p1, gdouble width);
void draw_node(cairo_t *cr, guint8 type, point pc);
