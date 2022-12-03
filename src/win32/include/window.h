#ifndef __window_h_
#define __window_h_

#include <Windows.h>

void win32_createWindow(unsigned int width, unsigned int height, const char *title);

void win32_changeWindowTitle(const char *title);
void win32_changeWindowColor(int r, int g, int b);

void win32_addUpdateFunction(void (*fn)());
void win32_run();

int win32_getWidth();
int win32_getHeight();

HWND win32_getHwnd();

#endif
