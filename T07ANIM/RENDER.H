/* FILENAME: RENDER.H
 * PROGRAMMER: AP6
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* Тип представления камеры в пространстве */
typedef struct
{
  VEC
    Loc,             /* точка расположения */
    Dir, Up, Right,  /* ориентационные вектора */
    At;              /* точка наблюдения */
} ap6CAMERA;

/* Тип представления объекта - набора вершин и треугольников и ребер */
typedef struct tagap6GOBJ
{
  VEC *V;      /* массив вершин объекта */
  INT NumOfV;  /* количество вершин */
  INT (*F)[3]; /* массив граней объекта */
  INT NumOfF;  /* количество граней */
} ap6GOBJ;

/* Глобальная камера */
extern ap6CAMERA AP6_RndCam;

/* Функция загрузки геометрического объекта */
BOOL AP6_RndGObjLoad( ap6GOBJ *GObj, CHAR *FileName );

/* Функция освобождения геометрического объекта */
VOID AP6_RndGObjFree( ap6GOBJ *GObj );

/* Функция отрисовки геометрического объекта */
VOID AP6_RndGObjDraw( ap6GOBJ *GObj, HDC hDC );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
