#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include <gtk/gtkx.h>

#include "include/window.h"

unsigned int width = 800;
unsigned int height = 600;
char *title = "VGUI App";

// GTK on activate function
static void onActivate(GtkApplication *app)
{
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), width, height);
  gtk_window_set_title(GTK_WINDOW(window), title);

  g_signal_connect(window, "destroy", gtk_main_quit, NULL);
  gtk_window_present(GTK_WINDOW(window));
}

// Creates a new Linux GTK window
void linux_createWindow(int w, int h, const char *t)
{
  width = w;
  height = h;
  title = t;

  GtkApplication *app = gtk_application_new("com.test.test", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(onActivate), NULL);

  char *fakeArgv[] = { (char*)title };
  g_application_run(G_APPLICATION(app), 0, fakeArgv);
}

// Runs the GTK window
void linux_run()
{
  gtk_main();
}

// Gets the native X11 display handle
Display *linux_getXDisplay()
{
  return gdk_x11_get_default_xdisplay();
}

// Gets the native X11 window handle
Window linux_getXWindow()
{
  return gdk_x11_get_default_root_xwindow();
}
