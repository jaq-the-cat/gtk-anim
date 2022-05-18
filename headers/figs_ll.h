#pragma once
#include "stdbool.h"
#include "figs.h"

// figures linked list
typedef struct _figures_node {
  figure *data;
  struct _figures_node *next;
} _figures_node;

typedef struct {
  _figures_node *head;
  guint16 len;
} figures;

figures figs_create();

void figs_add(figures *list, figure *data);

bool figs_remove_from_list(figures *list, figure *data);

void figs_delete_list(figures *list);
