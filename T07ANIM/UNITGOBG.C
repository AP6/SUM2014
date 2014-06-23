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
typedef struct tagap6UNIT_COW
{
  AP6_UNIT_BASE_FIELDS; /* Включение базовых полей */
  DBL ShiftX, ShiftY;
  INT Type;
  ap6GOBJ Cow;
} ap6UNIT_COW;

/* Функция инициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       ap6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitInit( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  AP6_RndGObjLoad(&Unit->Cow, "cow.object");
} /* End of 'GObjUnitInit' function */

/* Функция деинициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       ap6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitClose( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  AP6_RndGObjFree(&Unit->Cow);
} /* End of 'GObjUnitClose' function */

/* Функция обновления межкадровых параметров информационного
 * объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       ap6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitResponse( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    AP6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AP6_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
} /* End of 'GObjUnitResponse' function */

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ap6UNIT *Unit;
 *   - указатель на контекст анимации:
 *       ap6UNIT_INFO *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitRender( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  static DBL x, y, z;
  MATR WVP;

  x += Ani->JX;
  y += Ani->JY;
  z += Ani->JZ;

  Ani->MatrWorld = MatrMulMatr(MatrTranslate(x, y, 2 * z), MatrRotateY(y));;
  Ani->MatrView = MatrViewLookAt(VecSet(15, 15, 15), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrProjection1 = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                        -Ani->Hp / 2, Ani->Hp / 2,
                                        Ani->ProjDist, 1000);    

  
  WVP = MatrMulMatr(Ani->MatrWorld,
    MatrMulMatr(Ani->MatrView, Ani->MatrProjection1));

  
  glLoadMatrixd(WVP.A[0]);
/*  
 glBegin(GL_LINES);
    glColor3d(1, 0.5, 0.5);
    glVertex3d(-3, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0.5, 1, 0.5);
    glVertex3d(0, -3, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0.5, 0.5, 1);
    glVertex3d(0, 0, -3);
    glVertex4d(0, 0, 1, 0);
  glEnd();
*/  

  AP6_RndGObjDraw(&Unit->Cow, Ani->hDC);
} /* End of 'GObjUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ap6UNIT_INFO *) указатель на созданный объект анимации.
 */
ap6UNIT * AP6_GObjUnitCreate( VOID )
{
  ap6UNIT_COW *Unit;

  if ((Unit = (ap6UNIT_COW *)AP6_AnimUnitCreate(sizeof(ap6UNIT_COW))) == NULL)
    return NULL;

    /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)GObjUnitInit;
  Unit->Close = (VOID *)GObjUnitClose;
  Unit->Render = (VOID *)GObjUnitRender;
  Unit->Response = (VOID *)GObjUnitResponse;
  return (ap6UNIT *)Unit;
} /* End of 'AP6_GObjUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
