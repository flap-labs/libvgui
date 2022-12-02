#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "include/window.h"

// Globals
HWND hwnd;
MSG msg;

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
    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }
}

// Quits the application and closes the window
void win32_quit()
{
  if (hwnd == NULL)
  {
    printf("Error: Window already closed or an error occured.\n");
    exit(1);
  }

  DestroyWindow(hwnd);
}
