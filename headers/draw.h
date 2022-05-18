#pragma once
#include <gtk-3.0/gtk/gtk.h>

#define NT_ROOT 0
#define NT_OTHER 1

gdouble point_distance(gdouble x0, gdouble y0, gdouble x1, gdouble y1);
void draw_line(cairo_t *cr, GdkRGBA *color, gdouble x0, gdouble y0, gdouble x1, gdouble y1, gdouble width);
void draw_filled(cairo_t *cr, GdkRGBA *color, gdouble x0, gdouble y0, gdouble x1, gdouble y1, gdouble width);
void draw_white(cairo_t *cr, GdkRGBA *color, gdouble x0, gdouble y0, gdouble x1, gdouble y1, gdouble width);
void draw_empty(cairo_t *cr, GdkRGBA *color, gdouble x0, gdouble y0, gdouble x1, gdouble y1, gdouble width);
void draw_node(cairo_t *cr, guint8 type, gdouble xc, gdouble yc);
