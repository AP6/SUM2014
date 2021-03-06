/* FILENAME: ANIM.C
 * PROGRAMMER: AP6
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm")

#include "anim.h"

/* ��������� �������� �������� */
static ap6ANIM AP6_Anim;

/* ������ ��� ������������� �� ������� */
static INT64
  TimeFreq,  /* ������ ��������� � ������� */
  TimeStart, /* ����� ������ �������� */
  TimeOld,   /* ����� �������� ����� */
  TimePause, /* ����� ������� � ����� */
  TimeFPS;   /* ����� ��� ������ FPS */
static INT
  FrameCounter; /* ������� ������ */

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID AP6_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  AP6_Anim.hWnd = hWnd;
  AP6_Anim.hDC = CreateCompatibleDC(hDC);
  AP6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  AP6_Anim.W = 30;
  AP6_Anim.H = 30;
  AP6_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* �������������� ������ */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
} /* End of 'AP6_AnimInit' function */

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AP6_AnimClose( VOID )
{
  INT i;

  /* ������������ ������ ��-��� �������� �������� */
  for (i = 0; i < AP6_Anim.NumOfUnits; i++)
  {
    AP6_Anim.Units[i]->Close(AP6_Anim.Units[i], &AP6_Anim);
    free(AP6_Anim.Units[i]);
  }
  DeleteObject(AP6_Anim.hBmFrame);
  DeleteDC(AP6_Anim.hDC);
  memset(&AP6_Anim, 0, sizeof(ap6ANIM));
} /* End of 'AP6_AnimInit' function */

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID AP6_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(AP6_Anim.hWnd);

  /* ������������� ������ � ������ ����� */
  DeleteObject(AP6_Anim.hBmFrame);
  AP6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(AP6_Anim.hDC, AP6_Anim.hBmFrame);

  /* ���������� ������� */
  AP6_Anim.W = W;
  AP6_Anim.H = H;

  ReleaseDC(AP6_Anim.hWnd, hDC);
} /* End of 'AP6_AnimResize' function */

VOID AP6_AnimResponce( VOID )
{
  INT i;
    /* �������� */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* ��������� ����� ���������� � �������� */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* ��������� �������� ��������� */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* ������ */
        memcpy(AP6_Anim.JButsOld, AP6_Anim.JButs, sizeof(AP6_Anim.JButs));
        for (i = 0; i < 32; i++)
          AP6_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          AP6_Anim.JButsClick[i] = AP6_Anim.JButs[i] && !AP6_Anim.JButsOld[i];

        /* ��� */
        AP6_Anim.JX = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin - 1) - 1;
        AP6_Anim.JY = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASZ)
          AP6_Anim.JZ = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASR)
          AP6_Anim.JR = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin - 1) - 1;
        if (jc.wCaps & JOYCAPS_HASU)
          AP6_Anim.JU = 2.0 * (ji.dwUpos - jc.wUmin) / (jc.wUmax - jc.wUmin - 1) - 1;

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            AP6_Anim.JPOV = 0;
          else
            AP6_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }
}


/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AP6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  /* ���������� ����� */
  GetKeyboardState(AP6_Anim.Keys);
  for (i = 0; i < 256; i++)
    AP6_Anim.Keys[i] >>= 7;

  /* ���������� ������� */
  AP6_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* ���������� ����� */
  QueryPerformanceCounter(&li);

  /* ���������� ����� */
  AP6_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  AP6_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* ��������� ����� */
  if (AP6_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    AP6_Anim.DeltaTime = 0;
  }
  else
    AP6_Anim.DeltaTime = AP6_Anim.GlobalDeltaTime;

  AP6_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* ��������� FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    AP6_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* ����� "��������" ����� */
  TimeOld = li.QuadPart;

  /* ������� ���� */
  SelectObject(AP6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AP6_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AP6_Anim.hDC, RGB(0, 110, 0));
  Rectangle(AP6_Anim.hDC, 0, 0, AP6_Anim.W, AP6_Anim.H);

  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < AP6_Anim.NumOfUnits; i++)
    AP6_Anim.Units[i]->Response(AP6_Anim.Units[i], &AP6_Anim);

  /* ��������� �������� */
  for (i = 0; i < AP6_Anim.NumOfUnits; i++)
  {
    SelectObject(AP6_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AP6_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AP6_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(AP6_Anim.hDC, RGB(55, 155, 55));
    AP6_Anim.Units[i]->Render(AP6_Anim.Units[i], &AP6_Anim);
  }

  FrameCounter++;

  AP6_AnimResponce();
} /* End of 'AP6_AnimRender' function */

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AP6_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* ����� ����� */
  hDC = GetDC(AP6_Anim.hWnd);
  BitBlt(hDC, 0, 0, AP6_Anim.W, AP6_Anim.H, AP6_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AP6_Anim.hWnd, hDC);
} /* End of 'AP6_AnimCopyFrame' function */

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       ap6UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID AP6_AnimAddUnit( ap6UNIT *Unit )
{
  if (AP6_Anim.NumOfUnits < AP6_MAX_UNITS)
  {
    AP6_Anim.Units[AP6_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AP6_Anim);
  }
} /* End of 'AP6_AnimAddUnit' function */

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AP6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* ������� ����� */
  static RECT SaveRC;               /* ����������� ������ */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* ��������� ������ ������ ���� */
    GetWindowRect(AP6_Anim.hWnd, &SaveRC);

    /* ���������� � ����� �������� ��������� ���� */
    hmon = MonitorFromWindow(AP6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* �������� ���������� ��� �������� */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
    /* ��� ������ ��������:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(AP6_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(AP6_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* ��������������� ������ ���� */
    SetWindowPos(AP6_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'AP6_AnimFlipFullScreen' function */

/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID AP6_AnimSetPause( BOOL NewPauseFlag )
{
  AP6_Anim.IsPause = NewPauseFlag;
} /* End of 'AP6_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */
