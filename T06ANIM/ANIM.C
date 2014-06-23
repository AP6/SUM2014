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

/* Системный контекст анимации */
static ap6ANIM AP6_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

  /* инициализируем таймер */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
} /* End of 'AP6_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < AP6_Anim.NumOfUnits; i++)
  {
    AP6_Anim.Units[i]->Close(AP6_Anim.Units[i], &AP6_Anim);
    free(AP6_Anim.Units[i]);
  }
  DeleteObject(AP6_Anim.hBmFrame);
  DeleteDC(AP6_Anim.hDC);
  memset(&AP6_Anim, 0, sizeof(ap6ANIM));
} /* End of 'AP6_AnimInit' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(AP6_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(AP6_Anim.hBmFrame);
  AP6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(AP6_Anim.hDC, AP6_Anim.hBmFrame);

  /* Сохранение размера */
  AP6_Anim.W = W;
  AP6_Anim.H = H;

  ReleaseDC(AP6_Anim.hWnd, hDC);
} /* End of 'AP6_AnimResize' function */

VOID AP6_AnimResponce( VOID )
{
  INT i;
    /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(AP6_Anim.JButsOld, AP6_Anim.JButs, sizeof(AP6_Anim.JButs));
        for (i = 0; i < 32; i++)
          AP6_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          AP6_Anim.JButsClick[i] = AP6_Anim.JButs[i] && !AP6_Anim.JButsOld[i];

        /* Оси */
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


/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  /* Обновление ввода */
  GetKeyboardState(AP6_Anim.Keys);
  for (i = 0; i < 256; i++)
    AP6_Anim.Keys[i] >>= 7;

  /* Обновление таймера */
  AP6_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* Обновление кадра */
  QueryPerformanceCounter(&li);

  /* глобальное время */
  AP6_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  AP6_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* локальное время */
  if (AP6_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    AP6_Anim.DeltaTime = 0;
  }
  else
    AP6_Anim.DeltaTime = AP6_Anim.GlobalDeltaTime;

  AP6_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    AP6_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  /* очистка фона */
  SelectObject(AP6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AP6_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AP6_Anim.hDC, RGB(0, 110, 0));
  Rectangle(AP6_Anim.hDC, 0, 0, AP6_Anim.W, AP6_Anim.H);

  /* опрос на изменение состояний объектов */
  for (i = 0; i < AP6_Anim.NumOfUnits; i++)
    AP6_Anim.Units[i]->Response(AP6_Anim.Units[i], &AP6_Anim);

  /* рисование объектов */
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

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(AP6_Anim.hWnd);
  BitBlt(hDC, 0, 0, AP6_Anim.W, AP6_Anim.H, AP6_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AP6_Anim.hWnd, hDC);
} /* End of 'AP6_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       ap6UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_AnimAddUnit( ap6UNIT *Unit )
{
  if (AP6_Anim.NumOfUnits < AP6_MAX_UNITS)
  {
    AP6_Anim.Units[AP6_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AP6_Anim);
  }
} /* End of 'AP6_AnimAddUnit' function */

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(AP6_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(AP6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    /* для одного монитора:
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
    /* восстанавливаем размер окна */
    SetWindowPos(AP6_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'AP6_AnimFlipFullScreen' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_AnimSetPause( BOOL NewPauseFlag )
{
  AP6_Anim.IsPause = NewPauseFlag;
} /* End of 'AP6_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */
