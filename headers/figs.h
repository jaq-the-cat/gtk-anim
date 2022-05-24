#pragma once
#include <gtk-3.0/gtk/gtk.h>
#include "pmath.h"
#include <stdbool.h>

#define S_EMPTYCIRCLE 0
#define S_FILLEDCIRCLE 1
#define S_WHITECIRCLE 2
#define S_LINE 3

#define FIG(parent, p, shape, thickness, depth, r, g, b, children_count)\
  (figure) {\
    parent,\
    p, shape, thickness, depth,\
    (GdkRGBA) { r, g, b, 1 },\
    children_count,\
    malloc(sizeof(figure)*children_count)\
  }

#define PARENT(parent, coor) (dad) {parent, coor}

typedef struct dad {
  struct dad *parent;
  point *coor;
} dad;

typedef struct figure {
  struct dad parent;
  point coor; // endpoint x and y (root is parent node)
  guint8 shp   : 2; // shape of current node
  guint8 depth : 6;
  gdouble thickness;
  GdkRGBA color; // color, alpha always 1.0 (fully opaque)
  guint16 children_count;
  struct figure* children;
} figure;

typedef struct figure_nc { // no children
  point coor;
  guint8 shp   : 2;
  guint8 depth : 6; // max value is 16 (but 6 bits is 64, not using the rest)
  gdouble thickness;
  GdkRGBA color;
  guint16 children_count;
} figure_nc;

void fig_add_child(figure *parent, point offset, guint8 shp, guint8 depth, gdouble thickness, float r, float g, float b);

void fig_save_to_memory(figure *fig, char* filename);
void fig_load_from_memory(char* filename, figure *fig);
void fig_draw_nodes(figure *fig, cairo_t *cr, bool is_root);
void fig_draw_segment(figure *fig, cairo_t *cr);
void fig_free(figure *fig);

figure* fig_check_clicked(figure *fig, point p);
void move_figure_node(figure *fig, point p);
