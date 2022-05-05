#include <gtk-3.0/gtk/gtk.h>

void draw_line(cairo_t *cr, float x0, float y0, float x1, float y1) {
  cairo_set_line_width(cr, 5.0f);
  cairo_move_to(cr, x0, y0);
  cairo_line_to(cr, x1, y1);
  cairo_stroke(cr);
}
