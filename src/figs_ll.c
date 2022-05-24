#include "figs_ll.h"
#include "figs.h"

bool _compare_figs(figure *a, figure *b) {
  return a == b;
}

figures* _figs_make_node(figure *data) {
  figures *node = (figures*) malloc(sizeof(figures));
  node->fig = data;
  for (int i=0; i<16; i++) {
    node->depth[i].segment = NULL;
    node->depth[i].next = NULL;
  }
  node->next = NULL;
  return node;
}

figures_depth* _figsd_make_node(figure *segment) {
  figures_depth *node = (figures_depth*) malloc(sizeof(figures_depth));
  node->segment = segment;
  node->next = NULL;
  return node;
}

void _figs_remove_only(figures *list) {
  free(list);
  list = NULL;
}

void _figs_remove_first(figures *list) {
  figures *next = list->next;
  free(list);
  list = next;
}

void _figs_remove_node_after(figures *previous) {
  figures *next = previous->next->next;
  free(previous->next);
  previous->next = next;
}

bool _figs_remove_from_list(figures *node, figure *fig) {
  // return true if node found and deleted, false otherwise
  if (_compare_figs(node->next->fig, fig)) { // if the node after is the one to delete
    _figs_remove_node_after(node);
    return true;
  }
  else if (node->next->next != NULL) {
    return _figs_remove_from_list(node->next, fig);
  } else {
    return false;
  }
}

void _figs_delete_node(figures *node) {
  if (node != NULL) {
    if (node->next != NULL)
      _figs_delete_node(node->next);
    fig_free(node->fig); // TODO: Fix, double free error
    free(node);
  }
}

void build_depth(figure *fig, figures_depth *depth) {
  figure *child;
  for (int i=0; i<fig->children_count; i++) {
    child = &fig->children[i];
    // add child to depth list at index child->depth
    figsd_add(
        &depth[child->depth],
        child);

    build_depth(child, depth);
  }
}

void figs_add(figures *list, figure *fig) {
  // empty list
  if (list->fig == NULL) {
    list->fig = fig;
    build_depth(list->fig, list->depth);
  } else {
    figures *node;
    for (node = list; node->next != NULL; node = node->next); // move to last node
    node->next = _figs_make_node(fig);
    build_depth(node->next->fig, node->next->depth);
  }
}

void figsd_add(figures_depth *depth, figure *data) {
  // empty list
  if (depth->segment == NULL)
    depth->segment = data;
  else {
    figures_depth *node;
    for (node = depth; node->next != NULL; node = node->next); // move to last node
    node->next = _figsd_make_node(data);
  }
}

bool figs_remove_from_list(figures *list, figure *data) {
  if (list->fig == NULL) return false;

  if (_compare_figs(list->fig, data)) {
    // if the first item is the given figure
    if (list->fig != NULL && list->next == NULL)
      // if it's the only item
      _figs_remove_only(list);
    else
      // if it's not the only item
      _figs_remove_first(list);
  } else if (_figs_remove_from_list(list, data)) {
    // if managed to find the item in the middle of the list
  } else {
    // couldnt find the item
    return false;
  }
  // found the item
  return true;
}

void figs_delete_list(figures *list) {
  if (list->fig != NULL) {
    _figs_delete_node(list->next);
  }
}

// draw function
void figs_draw(figures *list, cairo_t *cr) {
  fig_draw(list->fig, cr);
  if (list->next != NULL)
    figs_draw(list->next, cr);
}

void _figs_draw_depth(figures_depth *node, cairo_t *cr) {
  if (node->segment == NULL)
    return;
  fig_draw_segment(node->segment, cr);
  if (node->next != NULL)
    _figs_draw_depth(node->next, cr);
}

void figs_draw_depth(figures *list, cairo_t *cr) {
  for (int i=0; i<16; i++) {
    _figs_draw_depth(&list->depth[i], cr);
  }
  fig_draw_nodes(list->fig, cr, false);
}

// check clicks
figure* _figs_check_click(figures *node, point p) {
  figure *returned = fig_check_clicked(node->fig, p);
  if (returned != NULL)
    // if found collided node
    return returned;
  if (node->next != NULL)
    // else, if you can check the others
    return _figs_check_click(node->next, p);
  else
    // this was the last node and u cant check any more -> return NULL
    return NULL;
}

figure* figs_check_click(figures *list, point p) {
  return _figs_check_click(list, p);
}
