/* FILENAME: SAMPLE.C
 * PROGRAMMER: AP6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* ��������� �������� ������� �������� */
typedef struct tagap6UNIT_CLOCK
{
  AP6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  DBL ShiftX, ShiftY;   /* �������� */
  INT Type;             /* ��� */
} ap6UNIT_CLOCK;


/* ��������� �������� ������� �������� */
typedef struct tagap6UNIT_INFO
{
  AP6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  IMAGE And, Xor;   /* �������� */
} ap6UNIT_INFO;


/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_CLOCK *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID ClockUnitInit( ap6UNIT_CLOCK *Unit, ap6ANIM *Ani )
{
} /* End of 'ClockUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_CLOCK *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID ClockUnitClose( ap6UNIT_CLOCK *Unit, ap6ANIM *Ani )
{
} /* End of 'ClockUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_CLOCK *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID ClockUnitResponse( ap6UNIT_CLOCK *Unit, ap6ANIM *Ani )
{
} /* End of 'ClockUnitResponse' function */


/* ������� ��������� �������.
 * ���������:
 *   - ��������:
 *       HDC hDC;
 *   - ���������� ������:
 *       INT Xc, Yc;
 *   - ����� � ������ �������:
 *       INT L, W;
 *   - ���� ���������:
 *       FLOAT Angle;
 * ������������ ��������: ���.
 */
VOID DrawArrow( HDC hDC, INT Xc, INT Yc, INT L, INT W, FLOAT Angle, INT R, INT G, INT B )
{
  INT i;
  POINT pts[] =
  {
    {0, -W}, {-W, 0}, {0, L}, {W, 0}
  }, pts_draw[sizeof pts / sizeof pts[0]];
  FLOAT si = sin(Angle), co = cos(Angle);

  for (i = 0; i < sizeof pts / sizeof pts[0]; i++)
  {
    pts_draw[i].x = Xc + (pts[i].x * co - pts[i].y * si);
    pts_draw[i].y = Yc - (pts[i].x * si + pts[i].y * co);
  }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(R, G, B));
  Polygon(hDC, pts_draw, sizeof pts / sizeof pts[0]);
} /* End of 'DrawArrow' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_CLOCK *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID ClockUnitRender( ap6UNIT_CLOCK *Unit, ap6ANIM *Ani )
{
  static DBL x, y;
  SYSTEMTIME st;

  x += Ani->JX;
  y += Ani->JY;

  GetLocalTime(&st);

  DrawArrow(Ani->hDC, Ani->W / 2 + x * 2, Ani->H / 2 + y * 2, 90, 18, (-(st.wHour % 12 + st.wMinute / 60.0) / 12.0) * 2 * PI, 0, 0, 255);
  DrawArrow(Ani->hDC, Ani->W / 2 + x * 2, Ani->H / 2 + y * 2, 120, 12, (-(st.wMinute + st.wSecond / 60.0) / 60.0) * 2 * PI, 0, 0, 255);
  DrawArrow(Ani->hDC, Ani->W / 2 + x * 2, Ani->H / 2 + y * 2, 130, 8, (-(st.wSecond + st.wMilliseconds / 1000.0) / 60.0) * 2 * PI, 0, 0, 255);
  DrawArrow(Ani->hDC, Ani->W / 2 + x * 2, Ani->H / 2 + y * 2, 140, 3, (-st.wMilliseconds / 1000.0) * 2 * PI, 0, 0, 255);
} /* End of 'AP6_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ap6UNIT *) ��������� �� ��������� ������ ��������.
 */
ap6UNIT * AP6_ClockUnitCreate( VOID )
{
  ap6UNIT_CLOCK *Unit;

  if ((Unit = (ap6UNIT_CLOCK *)AP6_AnimUnitCreate(sizeof(ap6UNIT_CLOCK))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)ClockUnitInit;
  Unit->Close = (VOID *)ClockUnitClose;
  Unit->Response = (VOID *)ClockUnitResponse;
  Unit->Render = (VOID *)ClockUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (ap6UNIT *)Unit;
} /* End of 'AP6_ClockUnitCreate' function */

/***********************************************************/


/* ������� ������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������: 
 *       ap6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitInit( ap6UNIT_INFO *Unit, ap6ANIM *Ani )
{
  ImageLoad(&Unit->And, "LOGO_AND.BMP");
  ImageLoad(&Unit->Xor, "LOGO_XOR.BMP");
}


/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitRender( ap6UNIT_INFO *Unit, ap6ANIM *Ani )
{
  static DBL x, y;
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 10, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));

  y += Ani->JZ * 2;
  x += Ani->JR * 2;

  ImageDraw(&Unit->And, Ani->hDC, Ani->W - Unit->And.W + x, y, SRCAND);
  ImageDraw(&Unit->Xor, Ani->hDC, Ani->W - Unit->And.W + x, y, SRCINVERT);
} /* End of 'AP6_AnimUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ap6UNIT *) ��������� �� ��������� ������ ��������.
 */
ap6UNIT_INFO * AP6_InfoUnitCreate( VOID )
{
  ap6UNIT_INFO *Unit;

  if ((Unit = (ap6UNIT_INFO *)AP6_AnimUnitCreate(sizeof(ap6UNIT_INFO))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Render = (VOID *)InfoUnitRender;
  return Unit;
} /* End of 'AP6_InfoUnitCreate' function */










/* END OF 'SAMPLE.C' FILE */
