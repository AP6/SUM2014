/* FILENAME: RENDER.H
 * PROGRAMMER: AP6
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 10.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* ��� ������������� ������ � ������������ */
typedef struct
{
  VEC
    Loc,             /* ����� ������������ */
    Dir, Up, Right,  /* �������������� ������� */
    At;              /* ����� ���������� */
} ap6CAMERA;

/* ��� ������������� ������� - ������ ������ � ������������� � ����� */
typedef struct tagap6GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} ap6GOBJ;

/* ���������� ������ */
extern ap6CAMERA AP6_RndCam;

/* ��������� ������������� */
extern DBL
  AP6_RndWs, AP6_RndHs, /* ������� ����� � ������ */
  AP6_RndWp, AP6_RndHp, /* ������� ������� ������������� */
  AP6_RndProjDist;      /* ���������� �� ��������� �������� */

/* ������� */
extern MATR
  AP6_RndMatrWorld, /* ������� �������������� ������� �� */
  AP6_RndMatrView;  /* ������� �������������� ������� �� */

/* ������� �������������� �� ������� ������� ��������� � ���� */
POINT AP6_RndWorldToScreen( VEC P );

/* ������� �������� ��������������� ������� */
BOOL AP6_RndGObjLoad( ap6GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ��������������� ������� */
VOID AP6_RndGObjFree( ap6GOBJ *GObj );

/* ������� ��������� ��������������� ������� */
VOID AP6_RndGObjDraw( ap6GOBJ *GObj, HDC hDC );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
