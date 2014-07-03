/* FILENAME: UNIT.C
 * PROGRAMMER: AP6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ap6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AP6_AnimUnitInit( ap6UNIT *Unit, ap6ANIM *Ani )
{
} /* End of 'AP6_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ap6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AP6_AnimUnitClose( ap6UNIT *Unit, ap6ANIM *Ani )
{
} /* End of 'AP6_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ap6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AP6_AnimUnitResponse( ap6UNIT *Unit, ap6ANIM *Ani )
{
} /* End of 'AP6_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ap6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AP6_AnimUnitRender( ap6UNIT *Unit, ap6ANIM *Ani )
{
} /* End of 'AP6_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ap6UNIT *) указатель на созданный объект анимации.
 */
ap6UNIT * AP6_AnimUnitCreate( INT Size )
{
  ap6UNIT *Unit;

  if (Size < sizeof(ap6UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* заполняем поля по-умолчанию */
  Unit->Size = Size;
  Unit->Init = AP6_AnimUnitInit;
  Unit->Close = AP6_AnimUnitClose;
  Unit->Response = AP6_AnimUnitResponse;
  Unit->Render = AP6_AnimUnitRender;
  return Unit;
} /* End of 'AP6_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

