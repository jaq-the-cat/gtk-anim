#include "pmath.h"

const point o = P(0, 0);

int main() {
  point torso = P(0, 4);
  point arm = P(1, -3);
  double angl = angle_between(torso, o, arm, o);

  // torso->arm: +161.56
  // arm->torso: -161.56

  printf("%lf\n", angl * 180 / G_PI);
  return 0;
}
