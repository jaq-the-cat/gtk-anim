#include "draw.h"
#include <math.h>

const GdkRGBA WHITE = {1, 1, 1, 1};
const GdkRGBA RED = {1, 0, 0, 1};
const GdkRGBA ORANGE = {1, 0.5, 0, 1};
const GdkRGBA BLACK = {0, 0, 0, 1};

#define _draw_arglist cairo_t *cr, GdkRGBA *color, gdouble x0, gdouble y0, gdouble x1, gdouble y1, gdouble width

#define DRAW_SETUP\
  gdk_cairo_set_source_rgba(cr, color);\
  cairo_set_line_width(cr, width);\
  cairo_move_to(cr, x0, y0)

void draw_line(_draw_arglist) {
  DRAW_SETUP;
  cairo_line_to(cr, x1, y1);
  cairo_stroke(cr);
}

#define SQR(_n) (_n)*(_n)

gdouble point_distance(gdouble x0, gdouble y0, gdouble x1, gdouble y1) {
  return sqrt(SQR(x1-x0) + SQR(y1-y0));
}

void draw_white(_draw_arglist) {
  draw_filled(cr, color, x0, y0, x1, y1, width);
  DRAW_SETUP; // and draw white circle
  gdk_cairo_set_source_rgba(cr, &WHITE);
  gdouble xc = (x0+x1)/2;
  gdouble yc = (y0+y1)/2;
  gdouble radius = point_distance(x0, y0, x1, y1)/2 - width;
  cairo_arc(cr, xc, yc, radius, 0, 2*G_PI); // main circle
  cairo_fill(cr);
  cairo_stroke(cr);
}

void draw_empty(_draw_arglist) {
  DRAW_SETUP;
  gdouble xc = (x0+x1)/2;
  gdouble yc = (y0+y1)/2;
  gdouble radius = point_distance(x0, y0, x1, y1)/2;
  cairo_arc(cr, xc, yc, radius, 0, 2*G_PI);
  cairo_stroke(cr);
}

void draw_filled(_draw_arglist) {
  DRAW_SETUP;
  gdouble xc = (x0+x1)/2;
  gdouble yc = (y0+y1)/2;
  gdouble radius = point_distance(x0, y0, x1, y1)/2;
  cairo_arc(cr, xc, yc, radius, 0, 2*G_PI);
  cairo_fill(cr);
  cairo_stroke(cr);
}

void draw_node(cairo_t *cr, guint8 type, gdouble xc, gdouble yc) {
  gdk_cairo_set_source_rgba(cr, &BLACK);
  cairo_set_line_width(cr, 1);
  cairo_move_to(cr, xc, yc);
  cairo_arc(cr, xc, yc, 5, 0, 2*G_PI);
  cairo_fill(cr);
  cairo_stroke(cr);

  gdk_cairo_set_source_rgba(cr, (type == NT_ROOT) ? &ORANGE : &RED);
  cairo_set_line_width(cr, 1);
  cairo_move_to(cr, xc, yc);
  cairo_arc(cr, xc, yc, 4, 0, 2*G_PI);
  cairo_fill(cr);
  cairo_stroke(cr);
}
