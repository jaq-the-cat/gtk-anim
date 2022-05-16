#pragma once
#include <gtk-3.0/gtk/gtk.h>

typedef enum {
  EmptyCircle,
  FilledCirlce,
  WhiteCircle,
  Line,
} shape;

typedef struct figure {
  guint32 x, y; // endpoint x and y (root is parent node)
  shape shp; // shape of current node
  guint16 thickness;
  GdkRGBA color; // color, alpha always 1.0 (fully opaque)
  guint16 children_count;
  struct figure* children;
} figure;

typedef struct figure_nc { // no children
  guint32 x, y;
  shape shp;
  guint16 thickness;
  GdkRGBA color;
  guint16 children_count;
} figure_nc;

void fig_serialize(figure fig, char* filename);
figure fig_unseralize(char* filename);
