#include "figs_ll.h"

bool _compare_figs(figure *a, figure *b) {
  return a == b;
}

_figures_node* _figs_make_node(figure *data) {
  _figures_node *node = (_figures_node*) malloc(sizeof(_figures_node));
  node->data = data;
  node->next = NULL;
  return node;
}

void _figs_remove_only(figures *list) {
  free(list->head);
  list->head = NULL;
}

void _figs_remove_first(figures *list) {
  _figures_node *next = list->head->next;
  free(list->head);
  list->head = next;
}

void _figs_remove_node_after(_figures_node *previous) {
  _figures_node *next = previous->next->next;
  free(previous->next);
  previous->next = next;
}

bool _figs_remove_from_list(int len, _figures_node *node, figure *data, int current_index) {
  // return true if node found and deleted, false otherwise
  if (_compare_figs(node->next->data, data)) { // if the node after is the one to delete
    _figs_remove_node_after(node);
    return true;
  }
  else if (current_index < len-1) {
    return _figs_remove_from_list(len, node->next, data, current_index+1);
  } else {
    return false;
  }
}

void _figs_delete_node(_figures_node *node) {
  if (node->next != NULL)
    _figs_delete_node(node->next);
  free(node);
}

figures figs_create() {
  figures list = {
    .head = NULL,
    .len = 0,
  };
  return list;
}

void figs_add(figures *list, figure *data) {
  // empty list
  if (list->len == 0)
    list->head = _figs_make_node(data);
  else {
    _figures_node *node;
    for (node = list->head; node->next != NULL; node = node->next); // move to last node
    node->next = _figs_make_node(data);
  }
  list->len++;
}

bool figs_remove_from_list(figures *list, figure *data) {
  if (list->len == 0) return false;

  if (_compare_figs(list->head->data, data)) {
    // if the first item is the given figure
    if (list->len == 1)
      // if it's the only item
      _figs_remove_only(list);
    else
      // if it's not the only item
      _figs_remove_first(list);
  } else if (_figs_remove_from_list(list->len, list->head, data, 0)) {
    // if managed to find the item in the middle of the list
  } else {
    // couldnt find the item
    return false;
  }
  // found the item
  list->len--;
  return true;
}

void figs_delete_list(figures *list) {
  if (list->len > 0) {
    _figs_delete_node(list->head);
    list->len = 0;
  }
}
