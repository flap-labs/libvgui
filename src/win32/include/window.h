#ifndef __window_h_
#define __window_h_

void win32_createWindow(unsigned int width, unsigned int height, const char *title);

void win32_changeSize(unsigned int width, unsigned int height);
void win32_changeTitle(const char *title);
void win32_changeColorRGB(int r, int g, int b);

void win32_run();

#endif
