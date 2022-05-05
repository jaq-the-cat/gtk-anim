#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>

#include "macros.h"

WIDGET_S(window, "main_window");
WIDGET_S(drawing_area, "drawing_area");

int main(int argc, char* argv[]) {
  // Init GTK
  gtk_init(&argc, &argv);

  // Load glade file
  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "app.glade", NULL);

  // Get main window from glade
  init_window(builder);

  // Setup drawing area event handlers
  init_drawing_area(builder);
  gtk_widget_add_events(drawing_area, 0);

  // Essential for GTK program
  gtk_builder_connect_signals(builder, NULL);
  g_object_unref(builder);

  // Start program
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}

gboolean render(GtkWidget *widget, cairo_t *cr, gpointer data) {
  GtkStyleContext *context = gtk_widget_get_style_context(widget);
  guint width = gtk_widget_get_allocated_width(widget);
  guint height = gtk_widget_get_allocated_height(widget);

  GdkRGBA color = {
    1.0, 1.0, 1.0, 1.0
  };
  /*gtk_style_context_get_color(context, gtk_style_context_get_state(context), &color);*/

  static float x;

  x += 1;

  gtk_render_background(context, cr, 0, 0, width, height);
  cairo_arc(cr, x, height/2.0, MIN(width, height) / 2.0, 0, 2 * G_PI);
  gdk_cairo_set_source_rgba(cr, &color);
  cairo_fill(cr);

  return FALSE;
}

void da_drag_begin(GtkGestureDrag *gesture, double x, double y, GtkWidget *area) {
  printf("drag begin\n");
  gtk_widget_queue_draw(area);
}

gboolean da_drag_motion(GtkGestureDrag *gesture, double x, double y, GtkWidget *area) {
  printf("drag motion\n");
  gtk_widget_queue_draw(area);
  return TRUE;
}

void da_drag_end(GtkGestureDrag *gesture, double x, double y, GtkWidget *area) {
  printf("drag end\n");
  gtk_widget_queue_draw(area);
}

// Glade
void exit_app() {
  printf("Exiting\n");
  gtk_main_quit();
}
