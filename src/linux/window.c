#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include <gtk/gtk.h>
#include <gtk/gtkx.h>

#include "include/window.h"
#include "include/opengl.h"

unsigned int width = 800;
unsigned int height = 600;
char *title = "VGUI App";

GtkWidget *gtkWindow;

static gboolean tick(GtkWidget *widget, GdkFrameClock *clock, gpointer data)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  linux_renderOpenGL();
  return TRUE;
}

// GTK on activate function
static void onActivate(GtkApplication *app)
{
  gtkWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(gtkWindow), width, height);
  gtk_window_set_title(GTK_WINDOW(gtkWindow), title);

  gtk_widget_show_all(gtkWindow);

  linux_createContext();

  g_signal_connect(gtkWindow, "destroy", gtk_main_quit, NULL);
  gtk_widget_add_tick_callback(gtkWindow, tick, NULL, NULL);
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
  return gdk_x11_display_get_xdisplay(gdk_display_get_default());
}

// Gets the native X11 window handle
Window linux_getXWindow()
{
  return gdk_x11_window_get_xid(gtk_widget_get_window(gtkWindow));
}
