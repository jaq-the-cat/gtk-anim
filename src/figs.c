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
  printf("READ: %d\n",read);
  if (read != sizeof(figure_nc)) {
    perror("Error reading file (corrupted?)");
  }
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

void fig_serialize(figure *fig, char* filename) {
  FILE *f = fopen(filename, "wb");
  if (!f) {
    perror("Error opening file in fig_seralize");
  }
  fig_write_to_file(fig, f);
  fclose(f);
}

figure fig_unseralize(char* filename) {
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

void fig_draw_recursive(figure *fig, cairo_t *cr, guint16 parent_x, guint16 parent_y, bool is_root) {
  printf("drawing with abs(%d, %d)\n", parent_x, parent_y);
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
