/* FILENAME: RENDER.C
 * PROGRAMMER: AP6
 * PURPOSE: 3D render handle module.
 * LAST UPDATE: 10.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"

/* ���������� ������ */
ap6CAMERA AP6_RndCam;

/* ��������� ������������� */
DBL
  AP6_RndWs = 30, AP6_RndHs = 30,   /* ������� ����� � ������ */
  AP6_RndWp = 4, AP6_RndHp = 3,     /* ������� ������� ������������� */
  AP6_RndProjDist = 5;              /* ���������� �� ��������� �������� */

/* ������� */
MATR
  AP6_RndMatrWorld = AP6_UNIT_MATR,      /* ������� �������������� ������� �� */
  AP6_RndMatrView = AP6_UNIT_MATR,       /* ������� �������������� ������� �� */
  AP6_RndMatrProjection = AP6_UNIT_MATR; /* ������� �������� */

static MATR
  AP6_RndMatrWorldViewProj;              /* �������� ������� �������������� */



POINT AP6_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* �������������� �� */
  Pp = VecMulMatr(P, AP6_RndMatrWorldViewProj);
  
  /* ������������� */
  Ps.x = ( Pp.X + 0.5) * (AP6_RndWs - 1);
  Ps.y = (-Pp.Y + 0.5) * (AP6_RndHs - 1);
  return Ps;
} /* End of 'AP6_RndWorldToScreen' function */


/* ������� �������������� �� ������� ������� ��������� � ����.
 * ���������:
 *   - �������� �����:
 *       VEC P;
 * ������������ ��������:
 *   (POINT) ���������� � �����.
 
POINT AP6_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;
  /* �������������� �� 
  Pp = VecMulMatr(P, MatrMulMatr(AP6_RndMatrWorld, AP6_RndMatrView));
  
  /* ������������� 
  Pp.X *= AP6_RndProjDist / Pp.Z;
  Pp.Y *= AP6_RndProjDist / Pp.Z;

  Ps.x = ( Pp.X + AP6_RndWp / 2) / AP6_RndWp * (AP6_RndWs - 1);
  Ps.y = (-Pp.Y + AP6_RndHp / 2) / AP6_RndHp * (AP6_RndHs - 1);
  return Ps;
} /* End of 'AP6_RndWorldToScreen' function */

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       ap6GOBJ *GObj;
 *   - ��� ����� (*.OBJ):
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL AP6_RndGObjLoad( ap6GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* �������� ������ */
  memset(GObj, 0, sizeof(ap6GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* ������� ���������� ������ � ������ */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* �������� ������ ��� ������� � ����� ��� ������ ������� ������
   * (memory bulk) */
  GObj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* ��������� ������ */
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
      sscanf(Buf, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
      sscanf(Buf, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 || 
      sscanf(Buf, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
      sscanf(Buf, "%i %i %i", &a, &b, &c); 
      a--;
      b--;
      c--;
      nf++;
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'AP6_RndGObjLoad' function */

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       ap6GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID AP6_RndGObjFree( ap6GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(ap6GOBJ));
} /* End of 'AP6_RndGObjFree' function */

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� �� ��������� ��� ����������� ���������:
 *       ap6GOBJ *GObj;
 *   - �������� ���������� ������:
 *       HDC hDC;
 * ������������ ��������: ���.
 */
VOID AP6_RndGObjDraw( ap6GOBJ *GObj, HDC hDC )
{
  INT i, j, s = 1;
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
} /* End of 'AP6_RndGObjDraw' function */

/* ������� �������������� �������� ������� ��������������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AP6_RndMatrSetup( VOID )
{
  AP6_RndMatrWorldViewProj =
    MatrMulMatr(MatrMulMatr(AP6_RndMatrWorld, AP6_RndMatrView),
      AP6_RndMatrProjection);
} /* End of 'AP6_RndMatrSetup' function */



/* END OF 'RENDER.C' FILE */
