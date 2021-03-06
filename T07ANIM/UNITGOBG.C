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
  //AP6_GeomLoad(&Unit->Cow, "e:\\Models\\Red Fox\\RED_FOX.OBJECT");
  //AP6_GeomLoad(&Unit->Cow, "e:\\Models\\Raccoon\\RACCON.OBJECT");
  AP6_GeomLoad(&Unit->Cow, "E:/Models/Elephant/elephant.obj");
  AP6_GeomTransform(&Unit->Cow, MatrScale(0.0030, 0.0030, 0.0030));
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
  static DBL time;
  MATR WVP;

  x += Ani->JX * Ani->DeltaTime * 3;
  y += Ani->JY * Ani->DeltaTime * 3 * Ani->JButs[3];
  z += Ani->JZ * Ani->DeltaTime * 3;
  r += Ani->JR * Ani->DeltaTime * 30;

  Ani->MatrWorld = MatrMulMatr(MatrTranslate(x, y, 2 * z), MatrRotateY(r));
  Ani->MatrView = MatrViewLookAt(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrProjection = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                       -Ani->Hp / 2, Ani->Hp / 2,
                                       Ani->ProjDist, 1000);    

  if ((time += Ani->DeltaTime) > 1)
  {
    time = 0;
    AP6_ShaderProg = AP6_ShadProgInit("a.vert", "a.frag");
  }

  for (i = -5; i <= 5; i++)
    for (j = -5; j <= 5; j++)
    {
      INT loc;

      glUseProgram(AP6_ShaderProg);
      loc = glGetUniformLocation(AP6_ShaderProg, "indI");
      if (loc != -1)
        glUniform1f(loc, i);
      loc = glGetUniformLocation(AP6_ShaderProg, "indJ");
      if (loc != -1)
        glUniform1f(loc, j);

      Ani->MatrWorld = MatrMulMatr(MatrTranslate(i * 2 + x, y, j * 3 + 2 * z), MatrRotateY(r));;
      /* ���������� ����� */
      WVP = MatrMulMatr(Ani->MatrWorld,
        MatrMulMatr(Ani->MatrView, Ani->MatrProjection));
      glLoadMatrixf(WVP.A[0]);

      AP6_GeomDraw(&Unit->Cow);
    }
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
