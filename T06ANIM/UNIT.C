/* FILENAME: UNIT.C
 * PROGRAMMER: AP6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AP6_AnimUnitInit( ap6UNIT *Unit, ap6ANIM *Ani )
{
} /* End of 'AP6_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AP6_AnimUnitClose( ap6UNIT *Unit, ap6ANIM *Ani )
{
} /* End of 'AP6_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AP6_AnimUnitResponse( ap6UNIT *Unit, ap6ANIM *Ani )
{
} /* End of 'AP6_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AP6_AnimUnitRender( ap6UNIT *Unit, ap6ANIM *Ani )
{
} /* End of 'AP6_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (ap6UNIT *) ��������� �� ��������� ������ ��������.
 */
ap6UNIT * AP6_AnimUnitCreate( INT Size )
{
  ap6UNIT *Unit;

  if (Size < sizeof(ap6UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* ��������� ���� ��-��������� */
  Unit->Size = Size;
  Unit->Init = AP6_AnimUnitInit;
  Unit->Close = AP6_AnimUnitClose;
  Unit->Response = AP6_AnimUnitResponse;
  Unit->Render = AP6_AnimUnitRender;
  return Unit;
} /* End of 'AP6_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

