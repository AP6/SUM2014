/* FILENAME: UNITINFO.C
 * PROGRAMMER: AP6
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 23.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "render.h"
#include "anim.h"

/* ��������� �������� ������� �������� */
typedef struct tagap6UNIT_COW
{
  AP6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  FLT ShiftX, ShiftY;
  INT Type;
  ap6GEOM Cow;
} ap6UNIT_COW;

/* ������� ������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID GObjUnitInit( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  AP6_GeomLoad(&Unit->Cow, "E:/Models/Inchworm/INCHWORM.OBJ");
  AP6_GeomTransform(&Unit->Cow, MatrScale(150, 150, 150));
  strcpy(Unit->Cow.ProgName, "Inchworm");

} /* End of 'GObjUnitInit' function */

/* ������� ��������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID GObjUnitClose( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  AP6_GeomFree(&Unit->Cow);
} /* End of 'GObjUnitClose' function */

/* ������� ���������� ����������� ���������� ���������������
 * ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID GObjUnitResponse( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  /*
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    AP6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AP6_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
  */
} /* End of 'GObjUnitResponse' function */

/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6UNIT_INFO *Ani;
 * ������������ ��������: ���.
 */
static VOID GObjUnitRender( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  INT i, j;
  static FLT x, y, z, r;

  x += Ani->JX * Ani->DeltaTime * 3;
  y += Ani->JY * Ani->DeltaTime * 3 * Ani->JButs[3];
  z += Ani->JZ * Ani->DeltaTime * 3;
  r += Ani->JR * Ani->DeltaTime * 30;

//  Ani->MatrWorld = MatrMulMatr(MatrTranslate(x + 1, y + 10, 2 * z  + 2), MatrRotateY(r));

  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  for (i = 0; i <= 1; i++)
    for (j = 0; j <= 1; j++)
    {
      INT loc;

      glUseProgram(Unit->Cow.ProgId); 
      loc = glGetUniformLocation(Unit->Cow.ProgId, "indI");
      if (loc != -1)
        glUniform1f(loc, i);
      loc = glGetUniformLocation(Unit->Cow.ProgId, "indJ");
      if (loc != -1)
        glUniform1f(loc, j);

      Ani->MatrWorld = MatrMulMatr(MatrTranslate(i * 2 + x, 5, j * 2 + z), MatrMulMatr(MatrRotateY(y), MatrRotateX(r)));
      AP6_GeomDraw(&Unit->Cow);
    }

  AP6_GeomDraw(&Unit->Cow);
} /* End of 'GObjUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ap6UNIT_INFO *) ��������� �� ��������� ������ ��������.
 */
ap6UNIT * AP6_GObjUnitCreate( VOID )
{
  ap6UNIT_COW *Unit;

  if ((Unit = (ap6UNIT_COW *)AP6_AnimUnitCreate(sizeof(ap6UNIT_COW))) == NULL)
    return NULL;

    /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)GObjUnitInit;
  Unit->Close = (VOID *)GObjUnitClose;
  Unit->Render = (VOID *)GObjUnitRender;
  Unit->Response = (VOID *)GObjUnitResponse;
  return (ap6UNIT *)Unit;
} /* End of 'AP6_GObjUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
