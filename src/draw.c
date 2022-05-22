#include "draw.h"
#include <math.h>

const GdkRGBA WHITE = {1, 1, 1, 1};
const GdkRGBA GRAY = {0.85, 0.85, 0.85, 1};

const GdkRGBA RED = {1, 0, 0, 1};
const GdkRGBA ORANGE = {1, 0.5, 0, 1};
const GdkRGBA BLACK = {0, 0, 0, 1};

#define _draw_arglist cairo_t *cr, GdkRGBA *color, point p0, point p1, gdouble width

#define DRAW_SETUP\
  gdk_cairo_set_source_rgba(cr, color);\
  cairo_set_line_width(cr, width);\
  cairo_move_to(cr, p0.x, p0.y)

void draw_line(_draw_arglist) {
  DRAW_SETUP;
  cairo_line_to(cr, p1.x, p1.y);
  cairo_stroke(cr);
}

gdouble point_distance(point p0, point p1) {
  return sqrt(SQR(p1.x-p0.x) + SQR(p1.y-p0.y));
}

void draw_white(_draw_arglist) {
  draw_filled(cr, color, p0, p1, width);
  DRAW_SETUP; // and draw white circle
  gdk_cairo_set_source_rgba(cr, &WHITE);
  gdouble xc = (p0.x+p1.x)/2;
  gdouble yc = (p0.y+p1.y)/2;
  gdouble radius = point_distance(p0, p1)/2 - width;
  cairo_arc(cr, xc, yc, radius, 0, 2*G_PI); // main circle
  cairo_fill(cr);
  cairo_stroke(cr);
}

void draw_empty(_draw_arglist) {
  DRAW_SETUP;
  gdouble xc = (p0.x+p1.x)/2;
  gdouble yc = (p0.y+p1.y)/2;
  gdouble radius = point_distance(p0, p1)/2;
  cairo_arc(cr, xc, yc, radius, 0, 2*G_PI);
  cairo_stroke(cr);
}

void draw_filled(_draw_arglist) {
  DRAW_SETUP;
  gdouble xc = (p0.x+p1.x)/2;
  gdouble yc = (p0.y+p1.y)/2;
  gdouble radius = point_distance(p0, p1)/2;
  cairo_arc(cr, xc, yc, radius, 0, 2*G_PI);
  cairo_fill(cr);
  cairo_stroke(cr);
}

void draw_node(cairo_t *cr, guint8 type, point c) {
  gdk_cairo_set_source_rgba(cr, &BLACK);
  cairo_set_line_width(cr, 1);
  cairo_move_to(cr, c.x, c.y);
  cairo_arc(cr, c.x, c.y, 5, 0, 2*G_PI);
  cairo_fill(cr);
  cairo_stroke(cr);

  gdk_cairo_set_source_rgba(cr, (type == NT_ROOT) ? &ORANGE : &RED);
  cairo_set_line_width(cr, 1);
  cairo_move_to(cr, c.x, c.y);
  cairo_arc(cr, c.x, c.y, 4, 0, 2*G_PI);
  cairo_fill(cr);
  cairo_stroke(cr);
}
