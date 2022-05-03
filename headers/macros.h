#pragma once
#include <gtk-3.0/gtk/gtk.h>

// Easier way to declare local widget
#define WIDGET(id) \
  GTK_WIDGET(gtk_builder_get_object(builder, id))
