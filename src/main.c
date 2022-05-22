#ifdef __unix__
// OS is Unix-based
#include <stdio.h>
#include <stdbool.h>
#include <gtk-3.0/gtk/gtk.h>

#include "macros.h"
#include "figs.h"
#include "figs_ll.h"

#define FRAME_MARGIN 40

#elif defined(_WIN32) || defined(WIN32)
// OS is Windows-based

#endif

#ifdef _PROD
// path to app.glade in production mode
#define GLADE_PATH "app.glade"
#else
#define GLADE_PATH "app.glade"
#endif

WIDGET_S(window, "main_window");
WIDGET_S(drawing_area, "drawing_area");

static bool is_moving_node = false;
static figure *moving_node;

static figures figs = FIGS_LL;

static figure fig1, fig2;

static cairo_surface_t *sfc;

void init_state() {
  fig1 = FIG(NULL, P(200, 200), S_LINE, 60, 0, 0.3, 1, 0);

  fig_add_child(&fig1, P(-30, 60), S_LINE, 60, 0, 0.3, 1); // left leg
  fig_add_child(&fig1, P(30, 60), S_LINE, 60, 0, 0.3, 1); // right leg
  fig_add_child(&fig1, P(0, -60), S_LINE, 60, 0, 0.3, 1); // torso

  fig_add_child(&fig1.children[2], P(0, -40), S_FILLEDCIRCLE, 60, 0, 0.3, 1); // head
  fig_add_child(&fig1.children[2], P(-30, 50), S_LINE, 60, 0, 0.3, 1); // left arm
  fig_add_child(&fig1.children[2], P(30, 50), S_LINE, 60, 0, 0.3, 1); // right arm

  /*fig_save_to_memory(&fig1, "fig1.gff");*/

  /*fig1 = fig_load_from_memory("fig1.gff");*/

  figs_add(&figs, &fig1);
}

int main(int argc, char* argv[]) {
  // Init GTK
  gtk_init(&argc, &argv);

  // Load glade file
  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, GLADE_PATH, NULL);

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
  // CAN BE OPTIMIZED
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

  // CAN PROBABLY BE OPTIMIZED
  GtkStyleContext *context = gtk_widget_get_style_context(widget);
  int width = gtk_widget_get_allocated_width(widget);
  int height = gtk_widget_get_allocated_height(widget);

  GdkRGBA color;
  gtk_style_context_get_color(context, gtk_style_context_get_state(context), &color);

  gtk_render_background(context, cr, 0, 0, width, height);

  // draw frame background

  GdkRGBA white = {1, 1, 1, 1};
  gdk_cairo_set_source_rgba(cr, &white);
  cairo_rectangle(cr, FRAME_MARGIN, FRAME_MARGIN, width-FRAME_MARGIN*2, height-FRAME_MARGIN*2);
  cairo_fill(cr);
  cairo_stroke(cr);

  figs_draw(&figs, cr);

  return FALSE;
}

gboolean da_button_press(GtkWidget *area, GdkEventButton *event, gpointer data) {
  // check if hovering over red thingey
  if (!is_moving_node) {
    moving_node = figs_check_click(&figs, P(event->x, event->y));
    is_moving_node = true;
  } else {
    moving_node = NULL;
    is_moving_node = false;
  }
  return TRUE;
}

gboolean da_motion(GtkWidget *area, GdkEventButton *event, gpointer data) {
  if (is_moving_node && moving_node != NULL) { // moving node
    move_figure_node(moving_node, P(event->x, event->y));
  }
  gtk_widget_queue_draw(area);
  return TRUE;
}

// Glade
void exit_app() {
  printf("Exiting\n");
  gtk_main_quit();
  figs_delete_list(&figs);
}
