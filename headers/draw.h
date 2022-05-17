#pragma once
#include <gtk-3.0/gtk/gtk.h>

#define NT_ROOT 0
#define NT_OTHER 1

void draw_line(cairo_t *cr, GdkRGBA *color, float x0, float y0, float x1, float y1, float width);
void draw_filled(cairo_t *cr, GdkRGBA *color, float x0, float y0, float x1, float y1, float width);
void draw_white(cairo_t *cr, GdkRGBA *color, float x0, float y0, float x1, float y1, float width);
void draw_empty(cairo_t *cr, GdkRGBA *color, float x0, float y0, float x1, float y1, float width);
void draw_node(cairo_t *cr, guint8 type, float xc, float yc);
