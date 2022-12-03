#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/opengl.h"
#include "include/window.h"

#include <X11/X.h>
#include <X11/Xutil.h>

#include <glad/glad.h>

#include <GL/glx.h>
#include <GL/gl.h>

// X11 globals
Display *display;
Window window;

typedef GLXContext (*glXCreateContextAttribsARB_type)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

// Creates an OpenGL context for Linux
void linux_createContext()
{
  GLint glxAttribs[] =
  {
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_DOUBLEBUFFER, 1,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_DEPTH_SIZE, 24,
    GLX_STENCIL_SIZE, 8,
    None
  };

  display = linux_getXDisplay();
  window = linux_getXWindow();
  int screen = XDefaultScreen(display);

  int fbCount = 0;
  GLXFBConfig *fbc = glXChooseFBConfig(display, screen, glxAttribs, &fbCount);

  glXCreateContextAttribsARB_type glXCreateContextAttribsARB = 0;
  glXCreateContextAttribsARB = (glXCreateContextAttribsARB_type*)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

  int contextAttribs[] =
  {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		None
	};

  GLXContext context = 0;
  context = glXCreateContextAttribsARB(display, fbc[0], NULL, 1, contextAttribs);

  glXMakeCurrent(display, window, context);
  gladLoadGL();
}

// Renders everything to the screen
void linux_renderOpenGL()
{
  glXSwapBuffers(display, window);
}
