/* FILENAME: RENDER.H
 * PROGRAMMER: AP6
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 23.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "shader.h"

/* ��� ������������� ������ � ������������ */
typedef struct
{
  VEC
    Loc,             /* ����� ������������ */
    Dir, Up, Right,  /* �������������� ������� */
    At;              /* ����� ���������� */
} ap6CAMERA;


/***
 * ������ � ��������������� ���������
 ***/

/* ��� ������������� ���������� ��������� */
typedef struct tagap6UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} ap6UV;

/* ������� ������������� ���������� ��������� */
__inline ap6UV AP6_UVSet( FLT U, FLT V )
{
  ap6UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'AP6_UVSet' function */

/* ��� ������������� ������� */
typedef struct tagvg4VERTEX
{
  VEC P;    /* ���������� */
  ap6UV T;  /* ���������� ���������� */
  VEC N;    /* ������� */
  COLOR C;  /* ���� */
} vg4VERTEX;


/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagap6PRIM_TYPE
{
  AP6_PRIM_TRIMESH, AP6_PRIM_GRID
} ap6PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagap6PRIM
{
  ap6PRIM_TYPE Type; /* ��� ��������� */
  INT
    NumOfV,     /* ���������� ������ */
    NumOfI,     /* ���������� �������� */
    GW, GH,     /* ������ ����� */
    Mtl,        /* ����� ��������� */
    Size;       /* ������ ���������� ������ � ������ */
  vg4VERTEX *V; /* ������ ������ */
  INT *I;       /* ������ �������� */

  /* ������ ��� ������ � OpenGL */
  INT
    Buffers[2],    /* ����� ������ � �������� */
    VertexBuffer;  /* ����� �������� ������ */
} ap6PRIM;

/* ��������� ��������� */
extern UINT AP6_ShaderProg;

/* ���� ��-��������� */
extern COLOR AP6_DefaultColor;

/* ������� �������� ��������� */
BOOL AP6_PrimCreate( ap6PRIM *P, ap6PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* ������� �������� ����� */
BOOL AP6_PrimCreateSphere( ap6PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* ������� �������� ��������� */
VOID AP6_PrimFree( ap6PRIM *P );

/* ������� ��������� ��������� */
VOID AP6_PrimDraw( ap6PRIM *P );

/* ������� �������������� �������� � ��������� */
VOID AP6_PrimAutoNormals( ap6PRIM *P );

/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagap6MATERIAL
{
  VEC Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
} ap6MATERIAL;

/* �������������� ������ - ����� ���������� */
typedef struct tagap6GEOM
{
  UINT ProgId;               /* ��������� ��������� */
  CHAR ProgName[MAX_STR];    /* ��� ���� ��������� ��������� */
  DBL ProgTime;              /* ����� ������� ������������ ��������� ��������� */
  INT NumOfPrims, NumOfMtls; /* ������� �������� */
  ap6MATERIAL *Mtls;         /* ������� ���������� */
  ap6PRIM *Prims;            /* ������ ���������� */
} ap6GEOM;

/* ������� ���������� ��������� � ������� */
INT AP6_GeomAddMaterial( ap6GEOM *G,
                         ap6MATERIAL *Mtl );

/* ������� ���������� ��������� � ������� */
INT AP6_GeomAddPrim( ap6GEOM *G, ap6PRIM *Prim );

/* ������� �������� ����� ����� */
BOOL AP6_PrimCreateHeightField( ap6PRIM *P, CHAR *FileName, FLT Height, FLT Scale );

/* ������� ������������ ��������������� ������� */
VOID AP6_GeomFree( ap6GEOM *G );

/* ������� ��������� */
VOID AP6_GeomDraw( ap6GEOM *G );

/* ������� �������� ��������������� ������� */
BOOL AP6_GeomLoad( ap6GEOM *G, CHAR *FileName );

/* ������� �������������� ������ */
VOID AP6_GeomTransform( ap6GEOM *G, MATR M );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
