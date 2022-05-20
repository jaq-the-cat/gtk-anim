#pragma once
#include <gtk-3.0/gtk/gtk.h>
#include <math.h>

#define P(x, y) (point) {x, y}
#define SQR(a) ((a)*(a))

typedef struct {
  gdouble x, y;
} point;

void limit_length(point centerp, point p, gdouble len, point *np);
double dot(point a, point b);
double mag(point p);
void rotate_around(point *p, point center, double angle);
