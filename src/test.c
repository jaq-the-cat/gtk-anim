#include "figs.h"

void print_fig(figure fig) {
  printf("{ %d %d %d %d } ", fig.x, fig.y, fig.thickness, fig.children_count);
  for (int i=0; i<fig.children_count; i++) {
    print_fig(fig.children[i]);
  }
  printf(" || ");
}

figure make_fig() {
  figure a = FIG(1, 1, S_LINE, 10, 1, 0, 1, 2);

  a.children[0] = FIG(0, 0, S_LINE, 5, 1, 0, 1, 1);
  a.children[0].children[0] = FIG(-2, 0, S_LINE, 5, 1, 0, 1, 0);
  a.children[1] = FIG(2, 2, S_LINE, 5, 1, 0, 1, 0);

  return a;
}

int main() {
  figure fig = make_fig();
  fig_serialize(fig, "figure.gff");
  figure fig_un = fig_unseralize("figure.gff");
  print_fig(fig);
  printf("\n FIG ^^ FIG_UN vv\n");
  print_fig(fig_un);
  return 0;
}