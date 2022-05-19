#pragma once
#include <gtk-3.0/gtk/gtk.h>

#define P(x, y) (point) {x, y}

#define S_EMPTYCIRCLE 0
#define S_FILLEDCIRCLE 1
#define S_WHITECIRCLE 2
#define S_LINE 3

#define FIG(parent, p, shape, thickness, r, g, b, children_count)\
  (figure) {\
    parent,\
    p, shape, thickness,\
    (GdkRGBA) { r, g, b, 1 },\
    children_count,\
    malloc(sizeof(figure)*children_count)\
  }

typedef struct {
  gdouble x, y;
} point;

typedef struct figure {
  struct figure* parent;
  point coor; // endpoint x and y (root is parent node)
  guint8 shp : 2; // shape of current node
  gdouble thickness;
  GdkRGBA color; // color, alpha always 1.0 (fully opaque)
  guint16 children_count;
  struct figure* children;
} figure;

typedef struct figure_nc { // no children
  point coor;
  guint8 shp : 2;
  gdouble thickness;
  GdkRGBA color;
  guint16 children_count;
} figure_nc;

void fig_save_to_memory(figure *fig, char* filename);
figure fig_load_from_memory(char* filename);
void fig_draw(figure *fig, cairo_t *cr);
void fig_free(figure *fig);

figure* fig_check_clicked(figure *fig, point p);
void move_figure_node(figure *fig, point p);
