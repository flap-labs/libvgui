#ifndef __frame_h_
#define __frame_h_

typedef struct
{
  int x, y;
  int width, height;
} Frame;

void frame_init();
void frame_render();
void frame_finalize();

Frame frame_new(int x, int y, int width, int height);
void frame_draw(Frame frame);

#endif
