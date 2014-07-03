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

/* Структура описания объекта анимации */
typedef struct tagap6UNIT_INST
{
  AP6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  FLT ShiftX, ShiftY;
  INT Type;
  ap6GEOM Inst;
} ap6UNIT_INST;

/* Функция инициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       ap6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InstUnitInit( ap6UNIT_INST *Unit, ap6ANIM *Ani )
{
  AP6_GeomLoad(&Unit->Inst, "e:\\Models\\Tarantula\\TRANTULA.OBJ");
  AP6_GeomTransform(&Unit->Inst, MatrScale(15, 15, 15));
  strcpy(Unit->Inst.ProgName, "spider");
} /* End of 'InstUnitInit' function */

/* Функция деинициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       ap6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InstUnitClose( ap6UNIT_INST *Unit, ap6ANIM *Ani )
{
  AP6_GeomFree(&Unit->Inst);
} /* End of 'InstUnitClose' function */

/* Функция обновления межкадровых параметров информационного
 * объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       ap6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ap6UNIT_INFO *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ap6UNIT_INFO *) указатель на созданный объект анимации.
 */
ap6UNIT * AP6_InstUnitCreate( VOID )
{
  ap6UNIT_INST *Unit;

  if ((Unit = (ap6UNIT_INST *)AP6_AnimUnitCreate(sizeof(ap6UNIT_INST))) == NULL)
    return NULL;

  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)InstUnitInit;
  Unit->Close = (VOID *)InstUnitClose;
  Unit->Render = (VOID *)InstUnitRender;
  Unit->Response = (VOID *)InstUnitResponse;
  return (ap6UNIT *)Unit;
} /* End of 'AP6_InstUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
