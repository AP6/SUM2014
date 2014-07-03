/* FILENAME: RENDER.H
 * PROGRAMMER: AP6
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 23.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "shader.h"

/* Тип представления камеры в пространстве */
typedef struct
{
  VEC
    Loc,             /* точка расположения */
    Dir, Up, Right,  /* ориентационные вектора */
    At;              /* точка наблюдения */
} ap6CAMERA;


/***
 * Работа с геометрическими объектами
 ***/

/* Тип представления текстурных координат */
typedef struct tagap6UV
{
  FLT U, V; /* компоненты текстурных координат */
} ap6UV;

/* Функиця инициализации текстурных координат */
__inline ap6UV AP6_UVSet( FLT U, FLT V )
{
  ap6UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'AP6_UVSet' function */

/* Теп представления вершины */
typedef struct tagvg4VERTEX
{
  VEC P;    /* координаты */
  ap6UV T;  /* текстурные координаты */
  VEC N;    /* нормаль */
  COLOR C;  /* цвет */
} vg4VERTEX;


/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagap6PRIM_TYPE
{
  AP6_PRIM_TRIMESH, AP6_PRIM_GRID
} ap6PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagap6PRIM
{
  ap6PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,     /* количество вершин */
    NumOfI,     /* количество индексов */
    GW, GH,     /* размер сетки */
    Mtl,        /* номер материала */
    Size;       /* размер выделенной памяти в байтах */
  vg4VERTEX *V; /* массив вершин */
  INT *I;       /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],    /* Буфер вершин и индексов */
    VertexBuffer;  /* Буфер описания вершин */
} ap6PRIM;

/* Шейдерная программа */
extern UINT AP6_ShaderProg;

/* Цвет по-умолчанию */
extern COLOR AP6_DefaultColor;

/* Функция создания примитива */
BOOL AP6_PrimCreate( ap6PRIM *P, ap6PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL AP6_PrimCreateSphere( ap6PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* Функция удаления примитива */
VOID AP6_PrimFree( ap6PRIM *P );

/* Функция отрисовки примитива */
VOID AP6_PrimDraw( ap6PRIM *P );

/* Функция перевычисления нормалей у примитива */
VOID AP6_PrimAutoNormals( ap6PRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagap6MATERIAL
{
  VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} ap6MATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagap6GEOM
{
  UINT ProgId;               /* шейдерная программа */
  CHAR ProgName[MAX_STR];    /* имя фйла шейдерной программы */
  DBL ProgTime;              /* время засечки перезагрузки шейдерной программы */
  INT NumOfPrims, NumOfMtls; /* размерв массивов */
  ap6MATERIAL *Mtls;         /* таблица материалов */
  ap6PRIM *Prims;            /* массив примитивов */
} ap6GEOM;

/* Функция добавления материала к объекту */
INT AP6_GeomAddMaterial( ap6GEOM *G,
                         ap6MATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT AP6_GeomAddPrim( ap6GEOM *G, ap6PRIM *Prim );

/* Функция создания карты высот */
BOOL AP6_PrimCreateHeightField( ap6PRIM *P, CHAR *FileName, FLT Height, FLT Scale );

/* Функция освобождения геометрического объекта */
VOID AP6_GeomFree( ap6GEOM *G );

/* Функция рисования */
VOID AP6_GeomDraw( ap6GEOM *G );

/* Функция загрузки геометрического объекта */
BOOL AP6_GeomLoad( ap6GEOM *G, CHAR *FileName );

/* Функция преобразования вершин */
VOID AP6_GeomTransform( ap6GEOM *G, MATR M );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
