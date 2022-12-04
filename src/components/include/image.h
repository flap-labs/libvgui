#ifndef __image_h_
#define __image_h_

#include <cglm/cglm.h>

typedef struct
{
  int x, y;
  int width, height;
  unsigned int texture;
  mat4 model;
} Image;

void image_finalize();
void image_render();

Image image_new(int x, int y, int width, int height, const char *filepath);
void image_draw(Image *image);

#endif
