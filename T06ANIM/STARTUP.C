/* FILENAME: STARTUP.C
 * PROGRAMMER: AP6
 * PURPOSE: Animation startup module
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

#define WND_CLASS_NAME "My Window Class Name"

/* ������ ��� ��������� ���� */
static INT
  AP6_MouseGlobalX, AP6_MouseGlobalY, /* ������� ���������� */
  AP6_MouseXOld, AP6_MouseYOld,       /* ����������� �� �������� ����� ���������� */
  AP6_MouseGlobalWheel;               /* ��������� ������ ���� */

/* ������ ������ */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );


/* ������� ��������� ����������� �������� ���������.
 * ���������:
 *   - ��� �������:
 *       INT Code;
 *   - �������� ��������� ('word parameter') -
 *     ��� ��������� �� ����:
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter') -
 *     (MSLLHOOKSTRUCT *) ��� �������� ���������;
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
static LRESULT CALLBACK AP6_MouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    AP6_MouseGlobalX = hs->pt.x;
    AP6_MouseGlobalY = hs->pt.y;
    break;
  case WM_MOUSEWHEEL:
    AP6_MouseGlobalWheel = (SHORT)HIWORD(hs->mouseData);
    break;
  }
  return 0;
} /* End of 'AP6_MouseHook' function */



/* ������� ������� ���������.
 * ���������:
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 *   - ���� ������ ���� (��. SW_SHOWNORMAL, SW_SHOWMINIMIZED, SW_***):
 *       INT ShowCmd;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  
  /* ����������� - �������� ������������ ������ ���� */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0; /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0; /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* ������� ���� - ��������� � ������� */
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* ������������ ����� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd = CreateWindow(WND_CLASS_NAME, "First Window Sample",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
    CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (w, h) - �� ��������� */
    NULL,                         /* ���������� ������������� ���� */
    NULL,                         /* ���������� ������������ ���� */
    hInstance,                    /* ���������� ���������� */
    NULL);                        /* ��������� �� �������������� ��������� */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /*** ���������� �������� ***/
  AP6_AnimAddUnit(AP6_CowUnitCreate());
  AP6_AnimAddUnit(AP6_InfoUnitCreate());

  /* ������ ����� ��������� ��������� */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ���������:
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;
  static BOOL pause;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    AP6_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    AP6_AnimResize(LOWORD(lParam), HIWORD(lParam));
  case WM_TIMER:
    AP6_AnimRender();
    AP6_AnimCopyFrame();
    return 0;
  case WM_CHAR:
    switch ((CHAR)wParam)
    {
    case 27:
      DestroyWindow(hWnd);
      return 0;
    case 'f':
      AP6_AnimFlipFullScreen();
      return 0;
    case 'p':
      AP6_AnimSetPause(pause = !pause);
      return 0;
    }
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    AP6_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    AP6_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'STARTUP.C' FILE */
