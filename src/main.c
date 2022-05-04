#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkglarea.h>
#include <cairo.h>
#include <GL/gl.h>

#include "macros.h"


/*GtkWidget *window;*/

WIDGET_S(GtkWidget, window, "main_window");

int main(int argc, char* argv[]) {
  // Init GTK
  gtk_init(&argc, &argv);

  // Load glade file
  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "app.glade", NULL);

  // Get main window from glade
  init_window(builder);

  // Essential for GTK program
  gtk_builder_connect_signals(builder, NULL);
  g_object_unref(builder);

  // Start program
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}

gboolean gl_render(GtkGLArea *area, GdkGLContext *context) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  return TRUE;
}

void gl_init(GtkGLArea *are) {

}

void gl_destroy(GtkGLArea *are) {

}

// Glade
void exit_app() {
  printf("Exiting\n");
  gtk_main_quit();
}
