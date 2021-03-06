/* FILENAME: UNITINFO.C
 * PROGRAMMER: AP6
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 23.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* ��������� �������� ������� �������� */
typedef struct tagap6UNIT_INFO
{
  AP6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
} ap6UNIT_INFO;

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
  wglUseFontBitmaps(Ani->hDC, 0, 256, 111);
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
  MATR mi = MatrIdenity();
  INT len;
  static CHAR Buf[1000];

  len = sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glUseProgram(0);
  glPushMatrix();
  glLoadMatrixf(mi.A[0]);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glRasterPos2d(-1, 0.95);
  glColor3d(1, 1, 1);
  glListBase(111);
  glCallLists(len, GL_UNSIGNED_BYTE, Buf);

  glRasterPos2d(-1, 0.88);
  glColor3d(1, 0.7, 0.5);
  glCallLists(sprintf(Buf, "J(%.3f %.3f %.3f [%.3f])-[%i]", Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JPOV), GL_UNSIGNED_BYTE, Buf);


  glPopMatrix();
  glPopAttrib();
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
  Unit->Close = (VOID *)InfoUnitClose;
  Unit->Render = (VOID *)InfoUnitRender;
  Unit->Response = (VOID *)InfoUnitResponse;
  return (ap6UNIT *)Unit;
} /* End of 'AP6_InfoUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
