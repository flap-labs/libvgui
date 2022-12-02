#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "include/opengl.h"
#include "include/window.h"

#include <gl/wglext.h>
#include <glad/glad.h>

// OpenGL function globals
typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext,
  const int *attribList);

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList,
  const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;
wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;

HDC hdc;

// Initializes the creation of an OpenGL context
void win32_initOpenGL()
{
  // Temporary window class
  WNDCLASSA wc = {0};
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = DefWindowProcA;
  wc.hInstance = GetModuleHandleA(NULL);
  wc.lpszClassName = "tmpOpenGLwnd";

  if (!RegisterClassA(&wc))
  {
    printf("Error: Failed to register a temporary OpenGL window class.\n");
    exit(1);
  }

  // Temporary window
  HWND tmpHwnd = CreateWindowExA(
    WS_EX_CLIENTEDGE,
    wc.lpszClassName,
    "Temporary OpenGL Window",
    0,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    NULL,
    NULL,
    wc.hInstance,
    NULL
  );

  if (!tmpHwnd)
  {
    printf("Error: Failed to create a temporary OpenGL window.\n");
    exit(1);
  }

  // Temporary OpenGL context creation
  HDC tmpHdc = GetDC(tmpHwnd);

  // Set the pixel format
  PIXELFORMATDESCRIPTOR pfd = {0};
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.cColorBits = 32;
  pfd.cAlphaBits = 8;
  pfd.iLayerType = PFD_MAIN_PLANE;
  pfd.cDepthBits = 24;
  pfd.cStencilBits = 8;

  int pf = ChoosePixelFormat(tmpHdc, &pfd);

  if (!pf)
  {
    printf("Error: Failed to find a pixel format.\n");
    exit(1);
  }

  if (!SetPixelFormat(tmpHdc, pf, &pfd))
  {
    printf("Error: Failed to set the pixel format.\n");
    exit(1);
  }

  // Create the context
  HGLRC tmpContext = wglCreateContext(tmpHdc);

  if (!tmpContext)
  {
    printf("Error: Failed to create an OpenGL context.\n");
    exit(1);
  }

  if (!wglMakeCurrent(tmpHdc, tmpContext))
  {
    printf("Error: Failed to make the OpenGL context current.\n");
    exit(1);
  }

  // Load the modern OpenGL context creation functions
  wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)
    wglGetProcAddress("wglCreateContextAttribsARB");

  wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)
    wglGetProcAddress("wglChoosePixelFormatARB");

  // Delete the temporary context
  wglMakeCurrent(tmpHdc, 0);
  wglDeleteContext(tmpContext);
  ReleaseDC(tmpHwnd, tmpHdc);
  DestroyWindow(tmpHwnd);
}

void win32_createContext()
{
  hdc = GetDC(win32_getHwnd());

  // Choose the pixel format the modern way
  int pfAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 24,
    WGL_STENCIL_BITS_ARB, 8,
    0 
  };

  int pf;
  UINT numFormats;
  wglChoosePixelFormatARB(hdc, pfAttribs, 0, 1, &pf, &numFormats);

  if (!numFormats)
  {
    printf("Error: Failed to set the modern OpenGL pixel format.\n");
    exit(1);
  }

  PIXELFORMATDESCRIPTOR pfd;
  DescribePixelFormat(hdc, pf, sizeof(pfd), &pfd);
  SetPixelFormat(hdc, pf, &pfd);

  // Specify the modern OpenGL context attributes
  int glAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
  };

  // Create the modern OpenGL context
  HGLRC context = wglCreateContext(hdc);
  wglMakeCurrent(hdc, context);
  
  // Load Glad
  gladLoadGL();
}

// Renders everything to the screen
void win32_renderOpenGL()
{
  SwapBuffers(hdc);
}
