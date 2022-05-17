#pragma once
#include <gtk-3.0/gtk/gtk.h>

#define S_EMPTYCIRCLE 0
#define S_FILLEDCIRCLE 1
#define S_WHITECIRCLE 2
#define S_LINE 3

#define FIG(x, y, shape, thickness, r, g, b, children_count)\
  (figure) {\
    x, y, shape, thickness,\
    (GdkRGBA) { r, g, b, 1 },\
    children_count,\
    malloc(sizeof(figure)*children_count)\
  }

typedef struct figure {
  guint32 x, y; // endpoint x and y (root is parent node)
  guint8 shp; // shape of current node
  guint16 thickness;
  GdkRGBA color; // color, alpha always 1.0 (fully opaque)
  guint16 children_count;
  struct figure* children;
} figure;

typedef struct figure_nc { // no children
  guint32 x, y;
  guint8 shp;
  guint16 thickness;
  GdkRGBA color;
  guint16 children_count;
} figure_nc;

void fig_serialize(figure fig, char* filename);
figure fig_unseralize(char* filename);
