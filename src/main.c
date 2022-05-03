#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>

#include "macros.h"


GtkWidget *window;

int main(int argc, char* argv[]) {
  // Init GTK
  gtk_init(&argc, &argv);

  // Load glade file
  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "app.glade", NULL);

  // Get main window from glade
  /*window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));*/
  window = WIDGET("main_window");

  // Essential for GTK program
  gtk_builder_connect_signals(builder, NULL);
  g_object_unref(builder);

  // Start program
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}

// Glade
void exit_app() {
  printf("Exiting\n");
  gtk_main_quit();
}
