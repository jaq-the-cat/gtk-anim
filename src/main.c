#include <stdio.h>
#include <stdbool.h>
#include <gtk-3.0/gtk/gtk.h>

#include "macros.h"
#include "figs.h"
#include "figs_ll.h"

WIDGET_S(window, "main_window");
WIDGET_S(drawing_area, "drawing_area");

static bool is_moving_node = false;
static figure *moving_node;

static figures figs = FIGS_LL;

static figure fig1, fig2;

static cairo_t *g_cr;

void init_state() {
  /*fig1 = FIG(100, 100, S_LINE, 12, 0, 0.3, 1, 3); // core*/
  /*fig1.children[0] = FIG(-15, 50, S_LINE, 12, 0, 0.3, 1, 0); // left leg*/
  /*fig1.children[1] = FIG(15, 50, S_LINE, 12, 0, 0.3, 1, 0); // right leg*/
  /*fig1.children[2] = FIG(0, -40, S_LINE, 12, 0, 0.3, 1, 3); // torso*/
  /*fig1.children[2].children[0] = FIG(-15, 30, S_LINE, 12, 0, 0.3, 1, 0); // left arm*/
  /*fig1.children[2].children[1] = FIG(15, 30, S_LINE, 12, 0, 0.3, 1, 0); // right arm*/
  /*fig1.children[2].children[2] = FIG(0, -30, S_FILLEDCIRCLE, 12, 0, 0.3, 1, 0); // head*/

  /*fig2 = FIG(200, 200, S_LINE, 12, 0, 0.3, 1, 3); // core*/
  /*fig2.children[0] = FIG(-15, 50, S_LINE, 12, 0, 0.3, 1, 0); // left leg*/
  /*fig2.children[1] = FIG(15, 50, S_LINE, 12, 0, 0.3, 1, 0); // right leg*/
  /*fig2.children[2] = FIG(0, -40, S_LINE, 12, 0, 0.3, 1, 3); // torso*/
  /*fig2.children[2].children[0] = FIG(-15, 30, S_LINE, 12, 0, 0.3, 1, 0); // left arm*/
  /*fig2.children[2].children[1] = FIG(15, 30, S_LINE, 12, 0, 0.3, 1, 0); // right arm*/
  /*fig2.children[2].children[2] = FIG(0, -30, S_FILLEDCIRCLE, 12, 0, 0.3, 1, 0); // head*/

  /*fig_save_to_memory(&fig1, "fig1.gff");*/
  /*fig_save_to_memory(&fig2, "fig2.gff");*/

  fig1 = fig_load_from_memory("fig1.gff");
  fig2 = fig_load_from_memory("fig2.gff");

  figs_add(&figs, &fig1);
  figs_add(&figs, &fig2);
}

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

  // Setup drawing area wevents
  gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);

  // initialize program state
  init_state();

  // Start program
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}

gboolean render(GtkWidget *widget, cairo_t *cr, gpointer data) {
  g_cr = cr;
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

  GtkStyleContext *context = gtk_widget_get_style_context(widget);
  int width = gtk_widget_get_allocated_width(widget);
  int height = gtk_widget_get_allocated_height(widget);

  GdkRGBA color;
  /*GdkRGBA color = {*/
    /*1.0, 1.0, 1.0, 1.0*/
  /*};*/
  gtk_style_context_get_color(context, gtk_style_context_get_state(context), &color);

  gtk_render_background(context, cr, 0, 0, width, height);

  figs_draw(&figs, cr);

  return FALSE;
}

gboolean da_button_press(GtkWidget *area, GdkEventButton *event, gpointer data) {
  // check if hovering over red thingey
  if (!is_moving_node) {
    moving_node = figs_check_click(&figs, event->x, event->y);
    is_moving_node = true;
  } else {
    moving_node = NULL;

    is_moving_node = false;
  }
  return TRUE;
}

gboolean da_motion(GtkWidget *area, GdkEventButton *event, gpointer data) {
  if (is_moving_node && moving_node != NULL) { // moving node
    move_figure_node(moving_node, event->x/2, event->y/2);
  } else { // not moving node
  }
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
