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
  FLT ShiftX, ShiftY;
  INT Type;
  ap6GEOM Cow;
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
 // AP6_GeomLoad(&Unit->Cow, "e:\\Models\\Red Fox\\RED_FOX.OBJECT");
//  AP6_GeomLoad(&Unit->Cow, "e:\\Models\\Raccoon\\RACCON.OBJECT");
//  AP6_GeomLoad(&Unit->Cow, "e:\\Models\\Tree\\tree.obj");
//  AP6_GeomLoad(&Unit->Cow, "E:/Models/Rattlesnake/RATTLER.OBJ");
//  AP6_GeomTransform(&Unit->Cow, MatrScale(10, 10, 10));
//  AP6_GeomLoad(&Unit->Cow, "E:/Models/Low poly Car-8/Low poly Car-8.obj");
//  AP6_GeomLoad(&Unit->Cow, "E:/Models/zebra/zebra.obj");
//  AP6_GeomLoad(&Unit->Cow, "e:\\Models\\Tarantula\\TRANTULA.OBJ");
  AP6_GeomTransform(&Unit->Cow, MatrScale(10, 10, 10));
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
  AP6_GeomFree(&Unit->Cow);
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
  /*
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    AP6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AP6_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);
  */
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
  Ani->MatrProjection = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2 + 0.5,
                                       -Ani->Hp / 2, Ani->Hp / 2 ,
                                       Ani->ProjDist, 1000);    

  if ((time += Ani->DeltaTime) > 1)
  {
    time = 0;
    AP6_ShaderProg = AP6_ShadProgInit("C.vert", "C.frag");
  }

    if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

/*
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
*/
      /* отладочный вывод */
      WVP = MatrMulMatr(Ani->MatrWorld,
        MatrMulMatr(Ani->MatrView, Ani->MatrProjection));
      glLoadMatrixf(WVP.A[0]);

      AP6_GeomDraw(&Unit->Cow);
//    }
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
