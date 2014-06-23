/* AP6, 05.06.2014 */


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include <windows.h>

#define WND_CLASS_NAME "My window class"
#define M_PI 3.14159265358979323846



LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, 
                               WPARAM wParam, LPARAM lParam );


/* */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "AP6", 
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL,
    hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    DispatchMessage(&msg);
  }
  return msg.wParam;
} /* End of 'WinMain' function */



/* */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, 
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  static HDC hMemDC;
  static HBITMAP hBm;
  PAINTSTRUCT ps;
  RECT rc;
  DOUBLE Fi, Tt;
  static int W, H;
  INT x, y, z, r = 300;
  SYSTEMTIME st;

  switch (Msg)
  {
  case WM_CHAR:
    if ((CHAR)wParam == 27)
      DestroyWindow(hWnd);
    return 0;  
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 30, 100, NULL);
    return 0;
  case WM_SIZE:
    H = HIWORD(lParam);
    W = LOWORD(lParam);
    GetClientRect(hWnd, &rc);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    
    SelectObject(hMemDC, hBm);
    return 0;
    
  case WM_LBUTTONDOWN:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_TIMER:
    hDC = GetDC(hWnd);

    GetClientRect(hWnd, &rc);
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));    
    SetDCPenColor(hMemDC, RGB(0, 0, 0));
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));

    Rectangle(hMemDC, 0, 0, rc.right, rc.bottom);
    
    GetLocalTime(&st);

    for (Tt = 0; Tt < 2 * M_PI; Tt += 0.05)
      for (Fi = 0; Fi < M_PI; Fi += 0.05)
      {
        y = r * cos(Tt);
        x = r * sin(Tt + st.wSecond) * sin(Fi);
        z = r * sin(Tt + st.wSecond) * cos(Fi);
        if (z > 0)
          SetPixel(hMemDC, x + rc.right / 2, y + rc.bottom / 2, RGB(0, 0, 255));
      }
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);

    ReleaseDC(hWnd, hDC);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  /*  
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    GetClientRect(hWnd, &rc);
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));    
    SetDCPenColor(hMemDC, RGB(0, 0, 0));
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));

    Rectangle(hMemDC, 0, 0, rc.right, rc.bottom);
    
    GetLocalTime(&st);

    for (Tt = 0; Tt < 2 * M_PI; Tt += 0.05)
      for (Fi = 0; Fi < M_PI; Fi += 0.05)
      {
        y = r * cos(Tt);
        x = r * sin(Tt + st.wSecond) * sin(Fi);
        z = r * sin(Tt + st.wSecond) * cos(Fi);
        if (z > 0)
          SetPixel(hMemDC, x + rc.right / 2, y + rc.bottom / 2, RGB(0, 0, 255));
      }
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
    */
  case WM_ERASEBKGND:
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 30);
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */