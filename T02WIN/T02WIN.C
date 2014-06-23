/* AP6, 02.06.2014 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include <windows.h>

#define WND_CLASS_NAME "My window class"



LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, 
                               WPARAM wParam, LPARAM lParam );


double r0( void )
{
  return (double)rand() / RAND_MAX;
}

void DrawEye(HDC hDC, INT Xm, INT Ym, INT Xc, INT Yc, INT W, INT H, INT R)
{
  int xe, ye;
  double len = sqrt((Xc - Xm) * (Xc - Xm) + (Yc - Ym) * (Yc - Ym)), si, co;
  SelectObject(hDC, GetStockObject(NULL_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(255, 255, 255));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  

}

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
  PAINTSTRUCT ps;
  RECT rc;
  POINT pt;
  INT a, x1, y1, x2, y2;

  switch (Msg)
  {
  case WM_CHAR:
    if((CHAR)wParam == 27)
      DestroyWindow(hWnd);
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    return 0;
  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    GetClientRect(hWnd, &rc);
    SelectObject(hDC, GetStockObject(NULL_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));    
    SetDCPenColor(hDC, RGB(0, 0, 0);
    SetDCBrushColor(hDC, RGB(255, 0, 0);

    Ellipse(hDC, 10, 40, 30, 20);
    Rectangle(hDC, 20, 100, 200, 300);
    /*
    srand(30);
    a =  rand();
    while (a--)
    {
      SetDCBrushColor(hDC, RGB(rand() % 255, rand() % 255, rand() % 255));
      x1 = r0() * rc.right;
      x2 = r0() * rc.right;
      y1 = r0() * rc.bottom;
      y2 = r0() * rc.bottom;
      Ellipse(hDC, x1, y1, x2, y2);
    }
    */
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, &pt);
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */