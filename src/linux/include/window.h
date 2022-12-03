#ifndef __window_h_
#define __window_h_

#include <X11/Xlib.h>
#include <X11/Xutil.h>

void linux_createWindow(int width, int height, const char *title);

void linux_changeWindowTitle(const char *newTitle);
void linux_changeWindowColor(int r, int g, int b);

void linux_run();
void linux_addUpdateFunction(void (*update)());

int linux_getWindowWidth();
int linux_getWindowHeight();

Display *linux_getXDisplay();
Window linux_getXWindow();

#endif
