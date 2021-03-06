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
typedef struct tagap6UNIT_INST
{
  AP6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  FLT ShiftX, ShiftY;
  INT Type;
  ap6GEOM Inst;
} ap6UNIT_INST;

/* ������� ������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InstUnitInit( ap6UNIT_INST *Unit, ap6ANIM *Ani )
{
  AP6_GeomLoad(&Unit->Inst, "e:\\Models\\Tarantula\\TRANTULA.OBJ");
  AP6_GeomTransform(&Unit->Inst, MatrScale(15, 15, 15));
  strcpy(Unit->Inst.ProgName, "spider");
} /* End of 'InstUnitInit' function */

/* ������� ��������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InstUnitClose( ap6UNIT_INST *Unit, ap6ANIM *Ani )
{
  AP6_GeomFree(&Unit->Inst);
} /* End of 'InstUnitClose' function */

/* ������� ���������� ����������� ���������� ���������������
 * ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InstUnitResponse( ap6UNIT_INST *Unit, ap6ANIM *Ani )
{
  /*
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    AP6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AP6_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
  */
} /* End of 'InstUnitResponse' function */

/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6UNIT_INFO *Ani;
 * ������������ ��������: ���.
 */
static VOID InstUnitRender( ap6UNIT_INST *Unit, ap6ANIM *Ani )
{
  static FLT x, y, z, r;

  x += Ani->JX * Ani->DeltaTime * 3;
  y += Ani->JY * Ani->DeltaTime * 3 * Ani->JButs[3];
  z += Ani->JZ * Ani->DeltaTime * 3;
  r += Ani->JR * Ani->DeltaTime * 30;

  Ani->MatrWorld = MatrMulMatr(MatrTranslate(x , y + 2, 2 * z + 2), MatrRotateY(r ));

  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  AP6_GeomDraw(&Unit->Inst);
} /* End of 'InstUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ap6UNIT_INFO *) ��������� �� ��������� ������ ��������.
 */
ap6UNIT * AP6_InstUnitCreate( VOID )
{
  ap6UNIT_INST *Unit;

  if ((Unit = (ap6UNIT_INST *)AP6_AnimUnitCreate(sizeof(ap6UNIT_INST))) == NULL)
    return NULL;

  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)InstUnitInit;
  Unit->Close = (VOID *)InstUnitClose;
  Unit->Render = (VOID *)InstUnitRender;
  Unit->Response = (VOID *)InstUnitResponse;
  return (ap6UNIT *)Unit;
} /* End of 'AP6_InstUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
