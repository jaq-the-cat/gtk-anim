#include "figs.h"
#include "draw.h"
#include <stdbool.h>

void fig_write_to_file(figure *fig, FILE* f) {
  // remove children so it doesnt write junk
  figure_nc no_child = (figure_nc) {
    fig->x, fig->y,
    fig->shp,
    fig->thickness,
    fig->color,
    fig->children_count,
  };
  fwrite(&no_child, sizeof(figure_nc), 1, f);
  for (int i=0; i<fig->children_count; i++) {
    fig_write_to_file(&fig->children[i], f);
  }
  // (fig 3) (fig 0) (fig 0) (fig 2) (fig) (fig)
}

figure fig_read_from_file(FILE* f){
  figure_nc fig_nc;

  int read = fread(&fig_nc, sizeof(figure_nc), 1, f);
  figure fig = (figure) {
    fig_nc.x, fig_nc.y,
    fig_nc.shp,
    fig_nc.thickness,
    fig_nc.color,
    fig_nc.children_count,
    malloc(sizeof(figure)*fig_nc.children_count) // allocate array of children in mem
  };

  for (int i=0; i<fig.children_count; i++) {
    fig.children[i] = fig_read_from_file(f);
  }

  return fig;
}

void fig_save_to_memory(figure *fig, char* filename) {
  FILE *f = fopen(filename, "wb");
  if (!f) {
    perror("Error opening file in fig_seralize");
  }
  fig_write_to_file(fig, f);
  fclose(f);
}

figure fig_load_from_memory(char* filename) {
  FILE *f = fopen(filename, "rb");
  if (!f) {
    perror("Error opening file in fig_unseralize (maybe it doesn't exist?)");
  }

  figure fig = fig_read_from_file(f);
  fclose(f);
  return fig;
}

// parameters for use in fig_draw_recursive
#define _params cr,\
  &child->color,\
  fig->x, fig->y,\
  child->x, child->y,\
  child->thickness\

void fig_draw_recursive(figure *fig, cairo_t *cr, bool is_root) {
  figure *child;
  for (int i=0; i<fig->children_count; i++) {
    child = &fig->children[i];
    switch (child->shp) {
      case S_LINE:
        draw_line(_params);
        break;
      case S_FILLEDCIRCLE:
        draw_filled(_params);
        break;
      case S_WHITECIRCLE:
        draw_white(_params);
        break;
      case S_EMPTYCIRCLE:
        draw_empty(_params);
        break;
      default:
        break;
    }
    fig_draw_recursive(child, cr, false);
  }
  if (is_root) {
    draw_node(cr, NT_ROOT, fig->x, fig->y);
  } else {
    draw_node(cr, NT_OTHER, fig->x, fig->y);
  }
}

void fig_draw(figure *fig, cairo_t *cr) {
  fig_draw_recursive(fig, cr, true);
}

figure* fig_check_clicked_recursive(figure *fig, gdouble x, gdouble y) {
  // radius = 5
  if (point_distance(x,  y, fig->x, fig->y) <= 5) {
    // collides with this figures coords
    return fig;
  } else {
    figure *returned; // NULL if doesnt collide, figure* otherwise
    for (int i=0; i<fig->children_count; i++) {
      returned = fig_check_clicked_recursive(&fig->children[i], x, y);
      if (returned != NULL) return returned;
    }
    return NULL;
  }
}

figure* fig_check_clicked(figure *fig, gdouble x, gdouble y) {
  return fig_check_clicked_recursive(fig, x, y);
}

// (a, b): center position
// (c, d): mouse position
// t = r / sqrt(a^2 - 2ac + b^2 - 2bd + c^2 + d^2)

void move_figure_node(figure *fig, gdouble x, gdouble y) {
  for (int i=0; i<fig->children_count; i++) {
    figure *child = &fig->children[i];
    move_figure_node(child,
      child->x + x - fig->x,
      child->y + y - fig->y);
  }
  fig->x = x;
  fig->y = y;
}

void fig_free(figure *fig) {
  figure *child;
  for (int i=0; i<fig->children_count; i++) {
    child = &fig->children[i];
    fig_free(child);
  }
}

void fig_add() {
  printf("Add figure\n");
}

void fig_load() {
  printf("Load figure\n");
}

void fig_new() {
  printf("New figure\n");
}

void fig_edit() {
  printf("Edit figure\n");
}

void cut() {
  printf("Cut\n");
}

void copy() {
  printf("Copy\n");
}

void paste() {
  printf("Paste\n");
}

void delete() {
  printf("Delete\n");
}

void color_set() {
  printf("Color set\n");
}
