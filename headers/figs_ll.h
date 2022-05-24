#pragma once
#include "stdbool.h"
#include "figs.h"

#define FIGS_MAP_HDR(func_name, params) void figs_##func_name(figures *list, params)
#define FIGS_LL (figures) { NULL, 0 }
#define SEP ,

// figures linked list

typedef struct figures {
  figure *data;
  struct figures *next;
} figures;

figures figs_create();

void figs_add(figures *list, figure *data);

bool figs_remove_from_list(figures *list, figure *data);

void figs_delete_list(figures *list);

// declare draw map func
void figs_draw(figures *list, cairo_t *cr);

// click check func
figure* figs_check_click(figures *list, point p);
