/* FILENAME: RENDER.C
 * PROGRAMMER: AP6
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 23.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"

/* Параметры проецирования */
DBL
  AP6_RndWs = 30, AP6_RndHs = 30,   /* размеры кадра в точках */
  AP6_RndWp = 4, AP6_RndHp = 3,     /* размеры обрасти проецирования */
  AP6_RndProjDist = 5;              /* расстояние до плоскости проекции */

/* Матрицы */
MATR
  AP6_RndMatrWorld = AP6_UNIT_MATR,      /* матрица преобразования мировой СК */
  AP6_RndMatrView = AP6_UNIT_MATR,       /* матрица преобразования видовой СК */
  AP6_RndMatrProjection = AP6_UNIT_MATR; /* матрица проекции */

static MATR
  AP6_RndMatrWorldViewProj;              /* Итоговая матрица преобразования */



POINT AP6_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* преобразование СК */
  Pp = VecMulMatr(P, AP6_RndMatrWorldViewProj);
  
  /* проецирование */
  Ps.x = ( Pp.X + 0.5) * (AP6_RndWs - 1);
  Ps.y = (-Pp.Y + 0.5) * (AP6_RndHs - 1);
  return Ps;
} /* End of 'AP6_RndWorldToScreen' function */


/* Функция преобразования из мировой системы коорлинат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 
POINT AP6_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;
  /* преобразование СК 
  Pp = VecMulMatr(P, MatrMulMatr(AP6_RndMatrWorld, AP6_RndMatrView));
  
  /* проецирование 
  Pp.X *= AP6_RndProjDist / Pp.Z;
  Pp.Y *= AP6_RndProjDist / Pp.Z;

  Ps.x = ( Pp.X + AP6_RndWp / 2) / AP6_RndWp * (AP6_RndWs - 1);
  Ps.y = (-Pp.Y + AP6_RndHp / 2) / AP6_RndHp * (AP6_RndHs - 1);
  return Ps;
} /* End of 'AP6_RndWorldToScreen' function */

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ap6GOBJ *GObj;
 *   - имя файла (*.OBJ):
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL AP6_RndGObjLoad( ap6GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* обнулили данные */
  memset(GObj, 0, sizeof(ap6GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* считаем количество вершин и граней */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* выделяем память под вершины и грани как единый участок памяти
   * (memory bulk) */
  GObj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* считываем данные */
  nv = 0;
  nf = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    DBL x, y, z;
    INT a, b, c;

    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      GObj->V[nv++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      if (sscanf(Buf, "f %i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
          sscanf(Buf, "f %i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 || 
          sscanf(Buf, "f %i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
          sscanf(Buf, "f %i %i %i", &a, &b, &c)) 
      {
        GObj->F[nf][0] = a - 1;
        GObj->F[nf][1] = b - 1;
        GObj->F[nf][2] = c - 1;
        nf++;
      }
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'AP6_RndGObjLoad' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ap6GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_RndGObjFree( ap6GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(ap6GOBJ));
} /* End of 'AP6_RndGObjFree' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       ap6GOBJ *GObj;
 *   - контекст устройства вывода:
 *       HDC hDC;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_RndGObjDraw( ap6GOBJ *GObj, HDC hDC )
{
  INT i = 0;
  glBegin(GL_LINES);
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n0 = GObj->F[i][0],
      n1 = GObj->F[i][1],
      n2 = GObj->F[i][2];
    
    glColor3d(1, 1, 1);
    glVertex3d(GObj->V[n0].X, GObj->V[n0].Y, GObj->V[n0].Z);
    glVertex3d(GObj->V[n1].X, GObj->V[n1].Y, GObj->V[n1].Z);
    glColor3d(1, 1, 1);
    glVertex3d(GObj->V[n1].X, GObj->V[n1].Y, GObj->V[n1].Z);
    glVertex3d(GObj->V[n2].X, GObj->V[n2].Y, GObj->V[n2].Z);
    glColor3d(1, 1, 1);
    glVertex3d(GObj->V[n0].X, GObj->V[n0].Y, GObj->V[n0].Z);
    glVertex3d(GObj->V[n2].X, GObj->V[n2].Y, GObj->V[n2].Z);   
  }
  glEnd();
  
  /*
  INT i, s = 1;
  POINT *pts;

  for (i = 0; i < GObj->NumOfV; i++)
  {
    pts[i] = AP6_RndWorldToScreen(GObj->V[i]);
  }
  for (i = 0; i < GObj->NumOfF; i++)
  {
    int n0 = GObj->F[i][0],
      n1 = GObj->F[i][1],
      n2 = GObj->F[i][2];
    MoveToEx(hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(hDC, pts[n1].x, pts[n1].y);
    LineTo(hDC, pts[n2].x, pts[n2].y);
    LineTo(hDC, pts[n0].x, pts[n0].y);
  }
  free(&pts);
  */
} /* End of 'AP6_RndGObjDraw' function */

/* Функция перевычисления итоговой матрицы преобразования.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_RndMatrSetup( VOID )
{
  AP6_RndMatrWorldViewProj =
    MatrMulMatr(MatrMulMatr(AP6_RndMatrWorld, AP6_RndMatrView),
      AP6_RndMatrProjection);
} /* End of 'AP6_RndMatrSetup' function */



/* END OF 'RENDER.C' FILE */
