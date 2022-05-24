#pragma once
#include "stdbool.h"
#include "figs.h"

#define FIGS_LL (figures) { NULL, NULL, NULL }

// figures linked list

typedef struct figures_depth {
  figure *segment;
  struct figures_depth *next;
} figures_depth;

typedef struct figures {
  figure *fig;
  figures_depth depth[16];
  struct figures *next;
} figures;

void figs_add(figures *list, figure *data);
void figsd_add(figures_depth *depth, figure *data);

bool figs_remove_from_list(figures *list, figure *data);

void figs_delete_list(figures *list);

// declare draw map func
void figs_draw(figures *list, cairo_t *cr);
void figs_draw_depth(figures *list, cairo_t *cr);

// click check func
figure* figs_check_click(figures *list, point p);
