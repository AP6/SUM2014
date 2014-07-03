/* FILENAME: GEOMLOAD.C
 * PROGRAMMER: AP6
 * PURPOSE: Geometry object load functions.
 * LAST UPDATE: 23.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "anim.h"

/***
 * Вспомогательные функции разбиения потока ввода на части
 ***/

/* Размер буфера */
#define MAX_BUF_SIZE 10000

/* Буфер ввода */
static CHAR Buf[MAX_BUF_SIZE];

/* Части буфера */
static INT NumOfParts;
static CHAR *Parts[MAX_BUF_SIZE / 2];

/* Функция разбиения строки на части.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) количество частей.
 */
static INT Split( VOID )
{
  INT i = 0;

  NumOfParts = 0;
  Parts[0] = Buf;
  while (Buf[i] != 0)
  {
    /* пропускаем пробелы */
    while (isspace(Buf[i]))
      i++;
    /* уходим по окончании строки */
    if (Buf[i] == 0)
      return NumOfParts;

    /* запоминаем начало новой части */
    Parts[NumOfParts++] = Buf + i;

    /* ищем конец слова */
    while (!isspace(Buf[i]) && Buf[i] != 0)
      i++;

    /* уходим по окончании строки */
    if (Buf[i] == 0)
      return NumOfParts;

    /* завершаем слово */
    Buf[i++] = 0;
  }
  return NumOfParts;
} /* End of 'Split' function */

/***
 * Массив переиндексации и хранения номеров вершин,
 * нормалей и текстурных координат
 ***/

/* Массив хранения */
struct
{
  INT
    Nv, Nn, Nt, /* номера индексов */
    Next;       /* указатель на следующий, или -1 */
} *VertexRefs;
INT NumOfVertexRefs, NumOfAllocedVertexRefs;

/* Массив указателей на первую хранимую вершину:
 *   VertexRefs[VertexRefsStart[Nv]] или -1 */
INT *VertexRefsStart;

/* Функция получения реального индекса вершины.
 * АРГУМЕНТЫ:
 *   - номера вершины, нормали и текстурных координат:
 *       INT Nv, Nn, Nt;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) найденный индекс.
 */
static INT GetVertexNo( INT Nv, INT Nt, INT Nn )
{
  INT *n = &VertexRefsStart[Nv];

  while (*n != -1 &&
         !(VertexRefs[*n].Nv == Nv &&
           VertexRefs[*n].Nn == Nn &&
           VertexRefs[*n].Nt == Nt))
    n = &VertexRefs[*n].Next;

  if (*n == -1)
  {
    if (NumOfVertexRefs >= NumOfAllocedVertexRefs)
      return 0; /* нет места для хранения вершины */
    *n = NumOfVertexRefs++;
    VertexRefs[*n].Nv = Nv;
    VertexRefs[*n].Nn = Nn;
    VertexRefs[*n].Nt = Nt;
    VertexRefs[*n].Next = -1;
  }
  return *n;
} /* End of 'GetVertexNo' function */

/* Части имени файла */
static CHAR
  ModelDrive[_MAX_DRIVE],
  ModelDir[_MAX_DIR],
  ModelFileName[_MAX_FNAME],
  ModelFileExt[_MAX_EXT],
  TexDrive[_MAX_DRIVE],
  TexDir[_MAX_DIR],
  TexFileName[_MAX_FNAME],
  TexFileExt[_MAX_EXT];

/* Функция загрузки материала.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ap6GEOM *G;
 *   - имя файла материалов:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID LoadMaterials( ap6GEOM *G, CHAR *FileName )
{
  FILE *F;
  ap6MATERIAL DefMat, Mat;
  static CHAR FName[_MAX_PATH];

  DefMat.Ka = VecSet(0.1, 0.1, 0.1);
  DefMat.Kd = VecSet(1.0, 1.0, 1.0);
  DefMat.Ks = VecSet(0.0, 0.0, 0.0);
  DefMat.Phong = 30;
  DefMat.TexNo = 0;
  DefMat.Trans = 1;
  strcpy(DefMat.Name, "Default Material SPR 2014");
  DefMat.MapD[0] = 0;
  Mat = DefMat;

  if (FName != NULL)
    _makepath(FName, ModelDrive, ModelDir, FileName, "");
  if (FName != NULL && (F = fopen(FName, "r")) != NULL)
  {
    /* считываем все сроки */
    while (fgets(Buf, sizeof(Buf), F) != NULL)
    {
      Split();
      if (NumOfParts > 1)
        if (strcmp(Parts[0], "Ka") == 0)
        {
          sscanf(Parts[1], "%f", &Mat.Ka.X);
          sscanf(Parts[2], "%f", &Mat.Ka.Y);
          sscanf(Parts[3], "%f", &Mat.Ka.Z);
        }
        else if (strcmp(Parts[0], "Kd") == 0)
        {
          sscanf(Parts[1], "%f", &Mat.Kd.X);
          sscanf(Parts[2], "%f", &Mat.Kd.Y);
          sscanf(Parts[3], "%f", &Mat.Kd.Z);
        }
        else if (strcmp(Parts[0], "Ks") == 0)
        {
          sscanf(Parts[1], "%f", &Mat.Ks.X);
          sscanf(Parts[2], "%f", &Mat.Ks.Y);
          sscanf(Parts[3], "%f", &Mat.Ks.Z);
        }
        else if (strcmp(Parts[0], "Ns") == 0)
          sscanf(Parts[1], "%f", &Mat.Phong);
        else if (strcmp(Parts[0], "D") == 0 ||
                 strcmp(Parts[0], "d") == 0 ||
                 strcmp(Parts[0], "Tr") == 0)
          sscanf(Parts[1], "%f", &Mat.Trans);
        else if (strcmp(Parts[0], "map_Kd") == 0)
        {
          _splitpath(Parts[NumOfParts - 1], TexDrive, TexDir, TexFileName, TexFileExt);
          _makepath(Mat.MapD, ModelDrive, ModelDir, TexFileName, ".bmp");
        }
        else if (strcmp(Parts[0], "newmtl") == 0)
        {
          AP6_GeomAddMaterial(G, &Mat);
          Mat = DefMat;
          strncpy(Mat.Name, Parts[1], sizeof(Mat.Name) - 1);
        }
    }
    fclose(F);
  }
  AP6_GeomAddMaterial(G, &Mat);
} /* End of 'LoadMaterials' function */

/* Вспомогательная макро-функция разбора индексов граней */
#define SCANF3(Str, Ref) \
  if (sscanf(Str, "%i/%i/%i", &Ref[0], &Ref[1], &Ref[2]) == 3) \
    Ref[0]--, Ref[1]--, Ref[2]--;                              \
  else                                                         \
    if (sscanf(Str, "%i//%i", &Ref[0], &Ref[2]) == 2)          \
      Ref[0]--, Ref[1] = -1, Ref[2]--;                         \
    else                                                       \
      if (sscanf(Str, "%i/%i", &Ref[0], &Ref[1]) == 2)         \
        Ref[0]--, Ref[1]--, Ref[2] = -1;                       \
      else                                                     \
        if (sscanf(Str, "%i", &Ref[0]) == 1)                   \
          Ref[0]--, Ref[1] = -1, Ref[2] = -1;                  \
        else                                                   \
          Ref[0] = -1, Ref[1] = -1, Ref[2] = -1;               \
  if (Ref[0] != -1 && Ref[0] < 0)                              \
    Ref[0] = vn + Ref[0] + 1;                                  \
  if (Ref[1] != -1 && Ref[1] < 0)                              \
    Ref[1] = vtn + Ref[1] + 1;                                 \
  if (Ref[2] != -1 && Ref[2] < 0)                              \
    Ref[2] = vnn + Ref[2] + 1


/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ap6GEOM *G;
 *   - имя файла материалов:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL AP6_GeomLoad( ap6GEOM *G, CHAR *FileName )
{
  INT vn = 0, vtn = 0, vnn = 0, fn = 0, pn = 0, size, i, p;
  FILE *F;
  /* читаемые данные */
  VEC *ReadV, *ReadN;
  ap6UV *ReadUV;
  INT (*ReadF)[3];
  /* хранение примитивов */
  struct
  {
    INT
      First, Last, /* первый и последний номера вершин примитива */
      Mtl;         /* материал примитива */
  } *PrimInfo;

  memset(G, 0, sizeof(ap6GEOM));
  /* разбиваем имя на части и открываем файл */
  _splitpath(FileName, ModelDrive, ModelDir, ModelFileName, ModelFileExt);
  if ((F = fopen(FileName, "rt")) == NULL)
    return FALSE;

  /* считаем количества */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'v' && Buf[1] == 't')
      vtn++;
    else if (Buf[0] == 'v' && Buf[1] == 'n')
      vnn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn += Split() - 3;
    else if (strncmp(Buf, "usemtl", 6) == 0)
      pn++;

  if (pn == 0)
    pn = 1; /* материалы не использовались */

  /* загружаем:
   *   вершины                        vn
   *   нормали                        vvn
   *   текстурные координаты          vtn
   *   треугольники                   fn
   *   примитивы                      pn
   * дополнительно:
   *   индексы (Vv, Vn, Vt) <- новые номера вершин  ? (vn + vt + vnn) * ???
   *   начальные индексы              vn
   */

  /* выделяем память под вспомогательные данные */
  size = 
    sizeof(VEC) * vn +                        /*   вершины               vn */
    sizeof(VEC) * vnn +                       /*   нормали               vnn */
    sizeof(ap6UV) * vtn +                     /*   текстурные координаты vtn */
    sizeof(INT [3]) * fn +                    /*   треугольники          fn */
    sizeof(PrimInfo[0]) * pn +                /*   примитивы             pn */
    sizeof(VertexRefs[0]) * (vn + vtn + vnn) + /*   индексы (Vv, Vn, Vt)  (vn + vt + vnn) */
    sizeof(INT) * vn;                         /*   начальные индексы     vn */
  if ((ReadV = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  memset(ReadV, 0, size);

  /* расставляем указатели */
  ReadN = ReadV + vn;
  ReadUV = (ap6UV *)(ReadN + vnn);
  ReadF = (INT (*)[3])(ReadUV + vtn);
  VertexRefsStart = (INT *)(ReadF + fn);
  PrimInfo = (VOID *)(VertexRefsStart + vn);
  VertexRefs = (VOID *)(PrimInfo + pn);
  NumOfAllocedVertexRefs = vn + vtn + vnn;
  NumOfVertexRefs = 0;
  /* начала списка индексов вершин ==> -1 */
  memset(VertexRefsStart, 0xFF, sizeof(INT) * vn);
  memset(VertexRefs, 0xFF, sizeof(VertexRefs[0]) * NumOfAllocedVertexRefs);

  /* второй проход - читаем геометрию */
  rewind(F);
  vn = 0;
  vtn = 0;
  vnn = 0;
  fn = 0;
  pn = 0;
  PrimInfo[0].First = 0;

  /* считаем количества */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      FLT x = 0, y = 0, z = 0;

      sscanf(Buf + 2, "%f%f%f", &x, &y, &z);
      ReadV[vn++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'v' && Buf[1] == 't')
    {
      FLT u = 0, v = 0;

      sscanf(Buf + 3, "%f%f", &u, &v);
      ReadUV[vtn++] = AP6_UVSet(u, v);
    }
    else if (Buf[0] == 'v' && Buf[1] == 'n')
    {
      FLT nx = 0, ny = 0, nz = 0;

      sscanf(Buf + 3, "%f%f%f", &nx, &ny, &nz);
      ReadN[vnn++] = VecNormalize(VecSet(nx, ny, nz));
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n0[3], n1[3], n[3], r0, r1, r;

      Split();
      SCANF3(Parts[1], n0);
      r0 = GetVertexNo(n0[0], n0[1], n0[2]);

      SCANF3(Parts[2], n1);
      r1 = GetVertexNo(n1[0], n1[1], n1[2]);

      for (i = 3; i < NumOfParts; i++)
      {
        SCANF3(Parts[i], n);
        r = GetVertexNo(n[0], n[1], n[2]);

        ReadF[fn][0] = r0;
        ReadF[fn][1] = r1;
        ReadF[fn][2] = r;
        r1 = r;
        fn++;
      }
    }
    else if (strncmp(Buf, "usemtl", 6) == 0)
    {
      Split();

      /* запоминаем номер последней грани */
      if (pn != 0)
        PrimInfo[pn - 1].Last = fn - 1;

      /* ищем материал */
      for (i = 0; i < G->NumOfMtls; i++)
        if (strcmp(Parts[1], G->Mtls[i].Name) == 0)
          break;
      if (i == G->NumOfMtls)
        PrimInfo[pn].Mtl = 0;
      else
        PrimInfo[pn].Mtl = i;
      PrimInfo[pn].First = fn;
      pn++;
    }
    else if (strncmp(Buf, "mtllib ", 7) == 0)
    {
      Split();
      LoadMaterials(G, Parts[1]);
    }

  /* у последнего примитива запоминаем номер последней грани */
  if (pn == 0)
  {
    PrimInfo[0].Last = fn - 1;
    PrimInfo[0].Mtl = 0;
    LoadMaterials(G, NULL);
  }
  else
    PrimInfo[pn - 1].Last = fn - 1;
  fclose(F);

  /* Формируем примитивы из прочитанных данных */
  AP6_DefaultColor = ColorSet(1, 1, 1);
  for (p = 0; p < pn; p++)
  {
    INT minv, maxv, j;
    ap6PRIM prim;
    BOOL is_need_normal = FALSE;

    minv = maxv = ReadF[PrimInfo[p].First][0];
    for (i = PrimInfo[p].First; i <= PrimInfo[p].Last; i++)
      for (j = 0; j < 3; j++)
      {
        if (minv > ReadF[i][j])
          minv = ReadF[i][j];
        if (maxv < ReadF[i][j])
          maxv = ReadF[i][j];
      }
    vn = maxv - minv + 1;
    fn = PrimInfo[p].Last - PrimInfo[p].First + 1;
    AP6_PrimCreate(&prim, AP6_PRIM_TRIMESH, vn, fn * 3);

    /* копируем вершины */
    for (i = 0; i < vn; i++)
    {
      INT n;

      prim.V[i].P = ReadV[VertexRefs[minv + i].Nv];
      if ((n = VertexRefs[minv + i].Nn) != -1)
        prim.V[i].N = ReadN[n];
      else
        is_need_normal = TRUE;
      if ((n = VertexRefs[minv + i].Nt) != -1)
        prim.V[i].T = ReadUV[n];
    }
    /* копируем грани */
    for (i = 0; i < fn; i++)
      for (j = 0; j < 3; j++)
        prim.I[i * 3 + j] = ReadF[PrimInfo[p].First + i][j] - minv;
    if (is_need_normal)
      AP6_PrimAutoNormals(&prim);
    prim.Mtl = PrimInfo[p].Mtl;
    AP6_GeomAddPrim(G, &prim);
  }
  /* освобождаем память из-под прочитанных данных */
  free(ReadV);
  return TRUE;
} /* End of 'AP6_GeomLoad' function */

/* END OF 'GEOMLOAD.C' FILE */
