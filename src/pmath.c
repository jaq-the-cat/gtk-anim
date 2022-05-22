#include "pmath.h"

// (a, b): center position
// (c, d): mouse position
// L(t) = ((1-t)a+tc, (1-t)b+td)
// t = r / sqrt(a^2 - 2ac + b^2 - 2bd + c^2 + d^2)
void limit_length(point centerp, point p, double len, point *np) {
  // Limit length of line so it matches the given length and stays in the same angle
  double a = centerp.x, b = centerp.y, c = p.x, d = p.y;
  double t = len / sqrt(a*a - 2*a*c + b*b - 2*b*d + c*c + d*d);
  np->x = (1-t)*a + t*c;
  np->y = (1-t)*b + t*d;
}

double dot(point a, point b) {
  return a.x * b.x + a.y * b.y;
}

double cross(point a, point b) {
  return a.x * b.y - a.y * b.x;
}

double mag(point p) {
  return sqrt(SQR(p.x) + SQR(p.y));
}

void rotate_around(point *p, point center, double angle) {
  // make points relative to origin
  double x = p->x - center.x;
  double y = p->y - center.y;

  // rotate
  double xr = x * cos(angle) - y * sin(angle);
  double yr = x * sin(angle) + y * cos(angle);

  // make relative to center again
  p->x = xr + center.x;
  p->y = yr + center.y;

}

double angle_between(point _from, point _from_origin, point _to, point _to_origin) {
  point from = P(_from.x - _from_origin.x, _from.y - _from_origin.y);
  point to = P(_to.x - _to_origin.x, _to.y - _to_origin.y);

  // angle in radians, points relative to (0, 0)
  double angle = acos(dot(from, to) / (mag(from)*mag(to)));

  if (isnan(angle))
    return 0;
  if (cross(from, to) < 0)
    return -angle;
  return angle;
}
