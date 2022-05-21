#pragma once
#include <gtk-3.0/gtk/gtk.h>
#include <math.h>

#define P(x, y) (point) {x, y}
#define SQR(a) ((a)*(a))

typedef enum {
  TOP_RIGHT,
  BOTTOM_RIGHT,
  BOTTOM_LEFT,
  TOP_LEFT,
} quadrant;

typedef struct {
  gdouble x, y;
} point;

void limit_length(point centerp, point p, gdouble len, point *np);
double dot(point a, point b);
double mag(point p);
void rotate_around(point *p, point center, double angle);
double angle_between(point _from, point _from_origin, point _to, point _to_origin);
