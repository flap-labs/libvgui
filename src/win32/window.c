#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "include/window.h"
#include "include/opengl.h"

#include <glad/glad.h>

// Globals
HWND hwnd;
MSG msg;
void (*updateFunc)();

int r = 210;
int g = 210;
int b = 210;

// Win32 window procedure
LRESULT CALLBACK wndproc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  switch (message)
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProcA(window, message, wparam, lparam);
  }

  return 0;
}

// Creates a new Win32 window
void win32_createWindow(unsigned int width, unsigned int height, const char *title)
{
  win32_initOpenGL();

  // Window class
  WNDCLASSEXA wc;
  wc.cbSize = sizeof(WNDCLASSEXA);
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
  wc.lpfnWndProc = wndproc;
  wc.hInstance = GetModuleHandleA(NULL);
  wc.lpszClassName = "window";
  wc.lpszMenuName = NULL;
  wc.hbrBackground = CreateSolidBrush(RGB(220, 220, 220));

  if (!RegisterClassExA(&wc))
  {
    printf("Error: Failed to register a Win32 window class.\n");
    exit(1);
  }

  // Create the window
  hwnd = CreateWindowExA(
    WS_EX_CLIENTEDGE,
    wc.lpszClassName,
    title,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    width,
    height,
    NULL,
    NULL,
    wc.hInstance,
    NULL
  );

  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);

  win32_createContext();
  glViewport(0, height - win32_getHeight(), width, win32_getHeight()); 
}

// Runs the application
void win32_run()
{
  if (hwnd == NULL)
  {
    printf("Error: Window already closed or an error occured.\n");
    exit(1);
  }

  while (GetMessageA(&msg, NULL, 0, 0) > 0)
  {
    glClearColor(r/255.0f, g/255.0f, b/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    updateFunc();
    win32_renderOpenGL();

    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }
  
  if (hwnd == NULL)
  {
    printf("Error: Window already closed or an error occured.\n");
    exit(1);
  }

  DestroyWindow(hwnd);
}

// Changes the Win32 window's title
void win32_changeWindowTitle(const char *newTitle)
{
  SetWindowTextA(hwnd, (LPCSTR)newTitle);
}

// Changes the Win32 window's background color
void win32_changeWindowColor(int red, int green, int blue)
{
  r = red;
  g = green;
  b = blue;
}

// Sets the update function for the window
void win32_addUpdateFunction(void (*fn)())
{
  updateFunc = fn;
}

// Gets the width of the Win32 window
int win32_getWidth()
{
  RECT rect;
  GetClientRect(hwnd, &rect);

  return rect.right - rect.left;
}

// Gets the height of the Win32 window
int win32_getHeight()
{
  RECT rect;
  GetClientRect(hwnd, &rect);

  return rect.bottom - rect.top;
}

// Gets the Win32 window handle
HWND win32_getHwnd()
{
  return hwnd;
}
