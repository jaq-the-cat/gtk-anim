#pragma once
#include <gtk-3.0/gtk/gtk.h>

// Easier way to declare local widget
#define WIDGET(id) \
  GTK_WIDGET(gtk_builder_get_object(builder, id))

// Widget with constructor outside of scope
#define WIDGET_S(type, varname, id) \
  type *varname; \
  void init_##varname(GtkBuilder* builder) { \
     varname = GTK_WIDGET(gtk_builder_get_object(builder, id)); \
  };
