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
          parent_x+fig->x, parent_y+fig->y,\
          parent_x+fig->x+child->x, parent_y+fig->y+child->y,\
          child->thickness\

void fig_draw_recursive(figure *fig, cairo_t *cr, gdouble parent_x, gdouble parent_y, bool is_root) {
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
    fig_draw_recursive(child, cr, parent_x+fig->x, parent_y+fig->y, false);
  }
  if (is_root) {
    draw_node(cr, NT_ROOT, parent_x+fig->x, parent_y+fig->y);
  } else {
    draw_node(cr, NT_OTHER, parent_x+fig->x, parent_y+fig->y);
  }
}

void fig_draw(figure *fig, cairo_t *cr) {
  fig_draw_recursive(fig, cr, fig->x, fig->y, true);
}

figure* fig_check_clicked_recursive(figure *fig, gdouble x, gdouble y, gdouble parent_x, gdouble parent_y) {
  // radius = 5
  if (point_distance(x,  y, fig->x + parent_x, fig->y + parent_y) <= 5) {
    // collides with this figures coords
    return fig;
  } else {
    figure *returned; // NULL if doesnt collide, figure* otherwise
    for (int i=0; i<fig->children_count; i++) {
      returned = fig_check_clicked_recursive(&fig->children[i],
          x, y,
          parent_x+fig->x, parent_y+fig->y);
      if (returned != NULL) return returned;
    }
    return NULL;
  }
}

figure* fig_check_clicked(figure *fig, gdouble x, gdouble y) {
  return fig_check_clicked_recursive(fig, x, y, fig->x, fig->y);
}

void move_figure_node(figure *fig, gdouble x, gdouble y) {
  /*fig->x = x;*/
  /*fig->y = y;*/
}

void fig_free(figure *fig) {
  figure *child;
  for (int i=0; i<fig->children_count; i++) {
    child = &fig->children[i];
    fig_free(child);
    free(fig->children);
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
