/* FILENAME: SAMPLE.C
 * PROGRAMMER: AP6
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "render.h"



/* ��������� �������� ������� �������� */
typedef struct tagap6UNIT_INFO
{
  AP6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  IMAGE And, Xor;       /* ����� ����������� �������� */
  DBL Shift;            /* ����� �� ������� */
  DBL Amp;              /* ��������� �������� */
  ap6GOBJ Info;
} ap6UNIT_INFO;

/* ��������� �������� ������� �������� */
typedef struct tagap6UNIT_COW
{
  AP6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  DBL ShiftX, ShiftY;   /* �������� */
  INT Type;             /* ��� */

} ap6UNIT_COW;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitInit( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
} /* End of 'CowUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitClose( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
} /* End of 'CowUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitResponse( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
} /* End of 'CowUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CowUnitRender( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  DBL
    x = Unit->ShiftX + sin(Ani->Time + Unit->ShiftX) * 30,
    y = Unit->ShiftY + sin(Ani->Time + Unit->ShiftY) * 30;

  if (Unit->Type)
    Rectangle(Ani->hDC, x, y, x + 30, y + 30);
  else
    Ellipse(Ani->hDC, x, y, x + 30, y + 30);
} /* End of 'CowUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ap6UNIT *) ��������� �� ��������� ������ ��������.
 */
ap6UNIT * AP6_CowUnitCreate( VOID )
{
  ap6UNIT_COW *Unit;

  if ((Unit = (ap6UNIT_COW *)AP6_AnimUnitCreate(sizeof(ap6UNIT_COW))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)CowUnitInit;
  Unit->Close = (VOID *)CowUnitClose;
  Unit->Response = (VOID *)CowUnitResponse;
  Unit->Render = (VOID *)CowUnitRender;
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  return (ap6UNIT *)Unit;
} /* End of 'AP6_CowUnitCreate' function */

/***********************************************************/


/* ������� ������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitInit( ap6UNIT_INFO *Unit, ap6ANIM *Ani )
{
  ImageLoad(&Unit->And, ".BMP");
  ImageLoad(&Unit->Xor, "LOGO_XOR.BMP");
  AP6_RndGObjLoad(&Unit->Info, "bear.object");
} /* End of 'InfoUnitInit' function */

/* ������� ��������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitClose( ap6UNIT_INFO *Unit, ap6ANIM *Ani )
{
  ImageFree(&Unit->And);
  ImageFree(&Unit->Xor);
} /* End of 'InfoUnitClose' function */

/* ������� ���������� ����������� ���������� ���������������
 * ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitResponse( ap6UNIT_INFO *Unit, ap6ANIM *Ani )
{
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    AP6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AP6_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
} /* End of 'InfoUnitResponse' function */

/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6UNIT_INFO *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitRender( ap6UNIT_INFO *Unit, ap6ANIM *Ani )
{
  INT x, y;
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 10, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));


  x = sprintf(Buf, "jx:% f jy:% f jz:% f jr:% f ju:% f POV: %i",
    Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JU, Ani->JPOV);
  for (y = 0; y < 32; y++)
    x += sprintf(Buf + x, "%s", Ani->JButs[y] ? "#" : "_");

  TextOut(Ani->hDC, 10, 50, Buf, x);

  x = (Ani->W - Unit->And.W) + Ani->JX * Unit->Amp * sin(Ani->Time * 0.30 + Unit->Shift);
  y = (Ani->H - Unit->And.H) + Ani->JY * Unit->Amp * cos(Ani->Time * 0.30 + Unit->Shift);
  AP6_RndGObjDraw(&Unit->Info, Ani->hDC );
  //  ImageDraw(&Unit->And, Ani->hDC, x, y, SRCAND);
//  ImageDraw(&Unit->Xor, Ani->hDC, x, y, SRCINVERT);
} /* End of 'InfoUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ap6UNIT_INFO *) ��������� �� ��������� ������ ��������.
 */
ap6UNIT * AP6_InfoUnitCreate( VOID )
{
  ap6UNIT_INFO *Unit;

  if ((Unit = (ap6UNIT_INFO *)AP6_AnimUnitCreate(sizeof(ap6UNIT_INFO))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Render = (VOID *)InfoUnitRender;
  Unit->Response = (VOID *)InfoUnitResponse;
  Unit->Shift = rand();
  Unit->Amp = 0;
  return (ap6UNIT *)Unit;
} /* End of 'AP6_InfoUnitCreate' function */

/* END OF 'SAMPLE.C' FILE */