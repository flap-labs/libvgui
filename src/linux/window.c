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
void (*updateFn)();
int updateFnExists = 0;
int windowFrameVisible = 1;

int r = 210;
int g = 210;
int b = 210;

GtkWidget *gtkWindow;

static gboolean tick(GtkWidget *widget, GdkFrameClock *clock, gpointer data)
{
  glClearColor(r/255.0f, g/255.0f, b/255.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  if (updateFnExists == 1)
  {
    updateFn();
  }

  linux_renderOpenGL();
  return TRUE;
}

// GTK on activate function
static void onActivate(GtkApplication *app)
{
  gtkWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(gtkWindow), width, height);
  gtk_window_set_title(GTK_WINDOW(gtkWindow), title);
  gtk_window_set_decorated(GTK_WINDOW(gtkWindow), windowFrameVisible);

  gtk_widget_show_all(gtkWindow);
  linux_createContext();

  g_signal_connect(gtkWindow, "destroy", gtk_main_quit, NULL);
  gtk_widget_add_tick_callback(gtkWindow, tick, NULL, NULL);
}

// Creates a new Linux GTK window
void linux_createWindow(int w, int h, const char *t, int frameVisible)
{
  width = w;
  height = h;
  title = t;

  windowFrameVisible = frameVisible;

  GtkApplication *app = gtk_application_new("com.test.test", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(onActivate), NULL);

  char *fakeArgv[] = { (char*)title };
  g_application_run(G_APPLICATION(app), 0, fakeArgv);
}

// Changes the title of the GTK window
void linux_changeWindowTitle(const char *newTitle)
{
  title = newTitle;
  gtk_window_set_title(gtkWindow, title);
}

// Changes the window's background color
void linux_changeWindowColor(int red, int green, int blue)
{
  r = red;
  g = green;
  b = blue;
}

// Runs the GTK window
void linux_run()
{
  gtk_main();
}

// Adds a function that gets called every frame
void linux_addUpdateFunction(void (*update)())
{
  updateFn = update;
  updateFnExists = 1;
}

// Gets the width of the GTK window
int linux_getWindowWidth()
{
  return width;
}

// Gets the width of the GTK window
int linux_getWindowHeight()
{
  return height;
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
