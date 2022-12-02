#ifndef __window_h_
#define __window_h_

#include <X11/Xlib.h>
#include <X11/Xutil.h>

void linux_createWindow(int width, int height, const char *title);
void linux_run();

Display *linux_getXDisplay();
Window linux_getXWindow();

#endif
