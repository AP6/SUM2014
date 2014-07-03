/* FILENAME: ANIM.C
 * PROGRAMMER: AP6
 * PURPOSE: Base animation module
 * LAST UPDATE: 13.06.2014
 */

#include <stdlib.h>
#include <time.h>
#include <windows.h>

#pragma comment(lib, "winmm")
#include <mmsystem.h>

#include "anim.h"

/* Системный контекст анимации */
ap6ANIM AP6_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

static INT
  AP6_MouseGlobalX, AP6_MouseGlobalY, 
  AP6_MouseXOld, AP6_MouseYOld, 
  AP6_MouseGlobalWheel; 

static HHOOK AP6_hMouseHook;

UINT AP6_ShaderProg;

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
  return CallNextHookEx(AP6_hMouseHook, Code, wParam, lParam);
} /* End of 'AP6_MouseHook' function */

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
BOOL AP6_AnimInit( HWND hWnd )
{
  int i;
  LARGE_INTEGER li;
  PIXELFORMATDESCRIPTOR pfd = {0};

  /* Устанавливаем оконные параметры */
  AP6_Anim.hDC = GetDC(hWnd);
  AP6_Anim.hWnd = hWnd;
  AP6_Anim.W = 30;
  AP6_Anim.H = 30;
  AP6_Anim.NumOfUnits = 0;

  /*** Инициализация OpenGL ***/

  /* описываем формат точки */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(AP6_Anim.hDC, &pfd);
  DescribePixelFormat(AP6_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(AP6_Anim.hDC, i, &pfd);

  /* создаем контекст построения */
  AP6_Anim.hRC = wglCreateContext(AP6_Anim.hDC);
  /* делаем текущими контексты */
  wglMakeCurrent(AP6_Anim.hDC, AP6_Anim.hRC);

  /* инициализируем расширения */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AP6_Anim.hRC);
    ReleaseDC(AP6_Anim.hWnd, AP6_Anim.hDC);
    memset(&AP6_Anim, 0, sizeof(ap6ANIM));
    return FALSE;
  }
  /* параметры OpenGL по-умолчанию */
  /*. . . */
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);


  /* инициализируем таймер */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;

  /* инициализируем захват сообщений от мыши */
  AP6_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, AP6_MouseHook, GetModuleHandle(NULL), 0);

  /* Начальные значения параметров проецирования */
  AP6_Anim.Wp = 4;
  AP6_Anim.Hp = 3;
  AP6_Anim.ProjDist = 1;
  AP6_Anim.ProjSize = 1;
//  AP6_Anim.FarClip = 1000;
  AP6_Anim.MatrWorld = AP6_Anim.MatrView = AP6_Anim.MatrProjection = MatrIdenity();
  return TRUE;
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

  AP6_ShadProgClose(AP6_ShaderProg);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AP6_Anim.hRC);
  ReleaseDC(AP6_Anim.hWnd, AP6_Anim.hDC);
/*  DeleteObject(AP6_Anim.hWnd, AP6_Anim.hDC); */
 //* ReleaseDC(AP6_Anim.hDC); */
  memset(&AP6_Anim, 0, sizeof(ap6ANIM));

  /* Отключение ловушки для сообщений для мыши */
  UnhookWindowsHookEx(AP6_hMouseHook);
} /* End of 'AP6_AnimInit' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_AnimResize( INT W, INT H )
{
  DBL ratio_x = 1, ratio_y = 1;
  /* Сохранение размера */
  AP6_Anim.W = W;
  AP6_Anim.H = H;
  /* Поле просмотра */
  glViewport(0, 0, W, H);

  if (W > H)
    ratio_x = (DBL)W / H;
  else 
    ratio_y = (DBL)H / W;
  AP6_Anim.Wp = AP6_Anim.ProjSize * ratio_x;
  AP6_Anim.Hp = AP6_Anim.ProjSize * ratio_y;

  AP6_Anim.MatrProjection =
      MatrProjection(-AP6_Anim.Wp / 2, AP6_Anim.Wp / 2,
                     -AP6_Anim.Hp / 2, AP6_Anim.Hp / 2,
                      AP6_Anim.ProjDist, 1000);
} /* End of 'AP6_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  MATR WVP;

  /* Обновление ввода */
  GetKeyboardState(AP6_Anim.Keys);
  for (i = 0; i < 256; i++)
    AP6_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    AP6_Anim.KeysClick[i] = AP6_Anim.Keys[i] && !AP6_Anim.KeysOld[i];
  memcpy(AP6_Anim.KeysOld, AP6_Anim.Keys, 256);

  AP6_Anim.MsWheel = AP6_MouseGlobalWheel;
  AP6_MouseGlobalWheel = 0;

  pt.x = AP6_MouseGlobalX;
  pt.y = AP6_MouseGlobalY;
  ScreenToClient(AP6_Anim.hWnd, &pt);
  AP6_Anim.MsX = pt.x;
  AP6_Anim.MsY = pt.y;
  
  AP6_Anim.MsDeltaX = AP6_MouseGlobalX - AP6_MouseXOld;
  AP6_Anim.MsDeltaY = AP6_MouseGlobalY - AP6_MouseYOld;
  AP6_MouseXOld = AP6_MouseGlobalX;
  AP6_MouseYOld = AP6_MouseGlobalY;

  /*Джойстик*/
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

        AP6_Anim.JX = ((INT)(AP6_Anim.JX * 30)) / 30.0;
        AP6_Anim.JY = ((INT)(AP6_Anim.JY * 30)) / 30.0;
        AP6_Anim.JZ = ((INT)(AP6_Anim.JZ * 30)) / 30.0;
        AP6_Anim.JR = ((INT)(AP6_Anim.JR * 30)) / 30.0;
        AP6_Anim.JU = ((INT)(AP6_Anim.JU * 30)) / 30.0;

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

  /* опрос на изменение состояний объектов */
  for (i = 0; i < AP6_Anim.NumOfUnits; i++)
    AP6_Anim.Units[i]->Response(AP6_Anim.Units[i], &AP6_Anim);

  /* очистка фона */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* рисуем оси координат */
  glUseProgram(0);
  AP6_Anim.MatrWorld = MatrIdenity();
  WVP = MatrMulMatr(AP6_Anim.MatrWorld, MatrMulMatr(AP6_Anim.MatrView, AP6_Anim.MatrProjection));
  glLoadMatrixf(WVP.A[0]);
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_TEXTURE_2D);
  glLineWidth(2);
  glBegin(GL_LINES);
    glColor3d(1, 0.5, 0.5);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0.5, 1, 0.5);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0.5, 0.5, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();
  glPopAttrib();

  /* рисование объектов */
  for (i = 0; i < AP6_Anim.NumOfUnits; i++)
    AP6_Anim.Units[i]->Render(AP6_Anim.Units[i], &AP6_Anim);

  FrameCounter++;
} /* End of 'AP6_AnimRender' function */


/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_AnimCopyFrame( VOID )
{
  /* вывод кадра */
  glFinish();
  SwapBuffers(AP6_Anim.hDC);
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
      rc.right - rc.left, rc.bottom - rc.top,
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
