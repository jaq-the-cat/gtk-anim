#include "figs.h"
#include "draw.h"
#include <math.h>

void fig_add_child(figure *parent, point offset, guint8 shp, guint8 depth, gdouble thickness, float r, float g, float b) {
  parent->children_count++;
  // reallocate to children_count+1
  parent->children = realloc(parent->children, (parent->children_count)*sizeof(figure));
  point p = P(parent->coor.x + offset.x, parent->coor.y + offset.y);
  parent->children[parent->children_count-1] = FIG(PARENT(&parent->parent, &parent->coor), p, shp, depth, thickness, r, g, b, 0);
}

void fig_remove_child(figure *parent, figure *child) {
  gboolean found = false;
  int i;
  for (i=0; i<parent->children_count; i++) {
    if (&parent->children[i] == child) {
      found = true;
      break;
    }
  }

  if (found) {
    if (i == parent->children_count-1) {
      // last child
      parent->children = realloc(parent->children, (parent->children_count-1)*sizeof(figure));
    } else {
      for (; i<parent->children_count-1; i++)
        // for each child, set it to be equal to the next
        parent->children[i] = parent->children[i+1];
    }
    parent->children_count--;
  }
}

void fig_write_to_file(figure *fig, FILE* f) {
  // remove children so it doesnt write junk
  figure_nc no_child = (figure_nc) {
    fig->coor,
    fig->shp,
    fig->depth,
    fig->thickness,
    fig->color,
    fig->children_count,
  };
  fwrite(&no_child, sizeof(figure_nc), 1, f);
  for (int i=0; i<fig->children_count; i++) {
    fig_write_to_file(&fig->children[i], f);
  }
}

void fig_read_from_file(FILE* f, figure *fig){
  figure_nc fig_nc;

  int read = fread(&fig_nc, sizeof(figure_nc), 1, f);
  fig->parent = PARENT(NULL, NULL);
  fig->coor = fig_nc.coor;
  fig->shp = fig_nc.shp;
  fig->depth = fig_nc.depth;
  fig->thickness = fig_nc.thickness;
  fig->color = fig_nc.color;
  fig->children_count = fig_nc.children_count;
  fig->children = malloc(sizeof(figure)*fig_nc.children_count);

  for (int i=0; i<fig->children_count; i++) {
    fig_read_from_file(f, &fig->children[i]);
    fig->children[i].parent = PARENT(&fig->parent, &fig->coor);
  }
}

void fig_save_to_memory(figure *fig, char* filename) {
  FILE *f = fopen(filename, "wb");
  if (!f) {
    perror("Error opening file in fig_seralize");
  }
  fig_write_to_file(fig, f);
  fclose(f);
}

void fig_load_from_memory(char* filename, figure *fig) {
  FILE *f = fopen(filename, "rb");
  if (!f) {
    perror("Error opening file in fig_unseralize (maybe it doesn't exist?)");
  }

  fig_read_from_file(f, fig);
  fclose(f);
}

void fig_draw_nodes(figure *fig, cairo_t *cr, bool is_root) {
  for (int i=0; i<fig->children_count; i++)
    fig_draw_nodes(&fig->children[i], cr, false);
  if (is_root) {
    draw_node(cr, NT_ROOT, fig->coor);
  } else {
    draw_node(cr, NT_OTHER, fig->coor);
  }
}

void fig_draw_segment(figure *fig, cairo_t *cr) {
  switch (fig->shp) {
    case S_LINE:
      draw_line(cr, &fig->color, *fig->parent.coor, fig->coor, fig->thickness);
      break;
    case S_FILLEDCIRCLE:
      draw_filled(cr, &fig->color, *fig->parent.coor, fig->coor, fig->thickness);
      break;
    case S_WHITECIRCLE:
      draw_white(cr, &fig->color, *fig->parent.coor, fig->coor, fig->thickness);
      break;
    case S_EMPTYCIRCLE:
      draw_empty(cr, &fig->color, *fig->parent.coor, fig->coor, fig->thickness);
      break;
    default:
      break;
  }
}

figure* fig_check_clicked_recursive(figure *fig, point p) {
  // radius = 5
  if (point_distance(p, fig->coor) <= 5) {
    // collides with this figures coords
    return fig;
  } else {
    figure *returned; // NULL if doesnt collide, figure* otherwise
    for (int i=0; i<fig->children_count; i++) {
      returned = fig_check_clicked_recursive(&fig->children[i], p);
      if (returned != NULL) return returned;
    }
    return NULL;
  }
}

figure* fig_check_clicked(figure *fig, point p) {
  return fig_check_clicked_recursive(fig, p);
}

void move_figure_node_children(figure *fig, point old_parent_point, point old_parent_parent_point) {
  if (old_parent_point.x == fig->parent.coor->x && old_parent_point.y == fig->parent.coor->y)
    // if the points are equal
    return;

  point old_parent_point_this = fig->coor;

  // Save data for length errors
  gdouble correct_length = point_distance(fig->coor, old_parent_point); 

  // Angle of node relative to parent
  // PROBLEM: old->parent->parent->coor also changes
  gdouble correct_angle = angle_between(
      old_parent_point, old_parent_parent_point,
      fig->coor, old_parent_point);

  /*
   * Fix floating-point errors
   */
  limit_length(*fig->parent.coor, fig->coor, correct_length, &fig->coor);

  // New angle of node relative to parent
  gdouble new_angle = angle_between(*fig->parent.coor, *fig->parent.parent->coor, fig->coor, *fig->parent.coor);

  // Get angle error and fix
  gdouble angle_error = correct_angle - new_angle;
  if (angle_error != 0)
    rotate_around(&fig->coor, *fig->parent.coor, angle_error);

  gdouble angle_after_fix = angle_between(*fig->parent.coor, *fig->parent.parent->coor, fig->coor, *fig->parent.coor);

  for (int i=0; i<fig->children_count; i++) {
    figure *child = &fig->children[i];
    move_figure_node_children(child, old_parent_point_this, old_parent_point);
  }
}

void move_figure_node_static(figure *fig, point p) {
  for (int i=0; i<fig->children_count; i++) {
    figure *child = &fig->children[i];
    move_figure_node_static(child,
        P(p.x + (child->coor.x - fig->coor.x),
          p.y + (child->coor.y - fig->coor.y)));
  }
  fig->coor.x = p.x;
  fig->coor.y = p.y;
}

void move_figure_node(figure *fig, point p) {
  if (fig->parent.coor == NULL) {
    // if its a root node, move the whole thing
    move_figure_node_static(fig, p);
  } else {
    // if its not a root node
    point old_figcoor = fig->coor;
    gdouble length = point_distance(*fig->parent.coor, fig->coor);
    limit_length(*fig->parent.coor, p, length, &fig->coor);

    // fig->x and fig->y now on proper position
    for (int i=0; i<fig->children_count; i++) {
      figure *child = &fig->children[i];
      gdouble angle = angle_between(
          old_figcoor, *fig->parent.coor,
          fig->coor, *fig->parent.coor);
      move_figure_node_children(child, old_figcoor, *fig->parent.coor);
    }
  }
}

void fig_free(figure *fig) {
  figure *child;
  for (int i=0; i<fig->children_count; i++) {
    child = &fig->children[i];
    fig_free(child);
  }
  free(fig->children);
  fig->children_count = 0;
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
