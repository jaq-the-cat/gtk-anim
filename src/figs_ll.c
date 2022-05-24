#include "figs_ll.h"

bool _compare_figs(figure *a, figure *b) {
  return a == b;
}

figures* _figs_make_node(figure *data) {
  figures *node = (figures*) malloc(sizeof(figures));
  node->data = data;
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

bool _figs_remove_from_list(figures *node, figure *data) {
  // return true if node found and deleted, false otherwise
  if (_compare_figs(node->next->data, data)) { // if the node after is the one to delete
    _figs_remove_node_after(node);
    return true;
  }
  else if (node->next->next != NULL) {
    return _figs_remove_from_list(node->next, data);
  } else {
    return false;
  }
}

void _figs_delete_node(figures *node) {
  if (node != NULL) {
    if (node->next != NULL)
      _figs_delete_node(node->next);
    fig_free(node->data); // TODO: Fix, double free error
    free(node);
  }
}

figures figs_create() {
  figures list = {
    .data = NULL,
    .next = NULL,
  };
  return list;
}

void figs_add(figures *list, figure *data) {
  // empty list
  if (list->data == NULL)
    list->data = data;
  else {
    figures *node;
    for (node = list; node->next != NULL; node = node->next); // move to last node
    node->next = _figs_make_node(data);
  }
}

bool figs_remove_from_list(figures *list, figure *data) {
  if (list->data == NULL) return false;

  if (_compare_figs(list->data, data)) {
    // if the first item is the given figure
    if (list->data != NULL && list->next == NULL)
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
  if (list->data != NULL) {
    _figs_delete_node(list->next);
  }
}

// draw function
void _figs_draw(figures *node, cairo_t *cr) {
  fig_draw(node->data, cr);
  if (node->next != NULL)
    _figs_draw(node->next, cr);
}

void figs_draw(figures *list, cairo_t *cr) {
  _figs_draw(list, cr);
}

// check clicks
figure* _figs_check_click(figures *node, point p) {
  figure *returned = fig_check_clicked(node->data, p);
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
