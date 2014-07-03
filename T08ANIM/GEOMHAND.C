/* FILENAME: GEOMHAND.C
 * PROGRAMMER: AP6
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 23.06.2014
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"

/* Функция добавления материала к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ap6GEOM *G;
 *   - добавляемый материал:
 *       ap6MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного материала или -1
 *         при ошибке.
 */
INT AP6_GeomAddMaterial( ap6GEOM *G, ap6MATERIAL *Mtl )
{
  INT i;
  ap6MATERIAL *M;

  /* ищем материал в библиотеке */
  for (i = 0; i < G->NumOfMtls; i++)
    if (strcmp(G->Mtls[i].Name, Mtl->Name)== 0)
    {
      G->Mtls[i] = *Mtl;
      return i;
    }

  if ((M = malloc(sizeof(ap6MATERIAL) * (G->NumOfMtls + 1))) == NULL)
    return -1;
  if (G->Mtls != NULL)
  {
    /* копируем старые данные */
    memcpy(M, G->Mtls, sizeof(ap6MATERIAL) * G->NumOfMtls);
    /* освобождаем память */
    free(G->Mtls);
  }
  /* добавляем */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'AP6_GeomAddMaterial' function */

/* Функция добавления примитива к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ap6GEOM *G;
 *   - добавляемый примитив:
 *       ap6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива или -1
 *         при ошибке.
 */
INT AP6_GeomAddPrim( ap6GEOM *G, ap6PRIM *Prim )
{
  ap6PRIM *P;

  if ((P = malloc(sizeof(ap6PRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* копируем старые данные */
    memcpy(P, G->Prims, sizeof(ap6PRIM) * G->NumOfPrims);
    /* освобождаем память */
    free(G->Prims);
  }
  /* добавляем */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'AP6_GeomAddPrim' function */

/* Функция освобождения.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ap6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_GeomFree( ap6GEOM *G )
{
  INT i;

  for (i = 0; i < G->NumOfPrims; i++)
    AP6_PrimFree(G->Prims + i);
  free(G->Prims);
  for (i = 0; i < G->NumOfMtls; i++)
    glDeleteTextures(1, &G->Mtls[i].TexNo);
  free(G->Mtls);
  memset(G, 0, sizeof(ap6GEOM));
} /* End of 'AP6_GeomFree' function */

/* Функция рисования.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ap6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_GeomDraw( ap6GEOM *G )
{
  INT i, loc;
  MATR WVP, MatrWorldInvTrans = {{{0}}};
  VEC V;
  UINT SaveProgId;

  /* вычислили матрицы преобразования */
  WVP = MatrMulMatr(AP6_Anim.MatrWorld,
    MatrMulMatr(AP6_Anim.MatrView, AP6_Anim.MatrProjection));
  MatrWorldInvTrans = MatrTranspose(MatrInverse(AP6_Anim.MatrWorld));
  /* отладочный вывод */
  glLoadMatrixf(WVP.A[0]);


  /* Перезагружаем шейдера раз в секунду */
  if (G->ProgName[0] != 0)
  {
    if (G->ProgTime + 1.0 < AP6_Anim.GlobalTime)
    {
      static CHAR Buf[2][MAX_STR];

      AP6_ShadProgClose(G->ProgId);
      sprintf(Buf[0], "SHADERS\\%s.vert", G->ProgName);
      sprintf(Buf[1], "SHADERS\\%s.frag", G->ProgName);
      G->ProgTime = AP6_Anim.GlobalTime;
      G->ProgId = AP6_ShadProgInit(Buf[0], Buf[1]);
    }
  }

  /* сохраняем шейдера */
  if (G->ProgId != 0)
  {
    SaveProgId = AP6_ShaderProg;
    AP6_ShaderProg = G->ProgId;
  }

  /* выбор программы шейдеров вывода примитивов */
  glUseProgram(AP6_ShaderProg);
  loc = glGetUniformLocation(AP6_ShaderProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.A[0]);

  loc = glGetUniformLocation(AP6_ShaderProg, "MatrWorldInverseTranspose");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MatrWorldInvTrans.A[0]);

  loc = glGetUniformLocation(AP6_ShaderProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AP6_Anim.MatrWorld.A[0]);

  loc = glGetUniformLocation(AP6_ShaderProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AP6_Anim.MatrView.A[0]);

  loc = glGetUniformLocation(AP6_ShaderProg, "Time");
  if (loc != -1)
    glUniform1f(loc, AP6_Anim.Time);

  V = VecSet(-AP6_Anim.MatrView.A[0][2],
             -AP6_Anim.MatrView.A[1][2],
             -AP6_Anim.MatrView.A[2][2]);
  loc = glGetUniformLocation(AP6_ShaderProg, "ViewDir");
  if (loc != -1)
    glUniform3fv(loc, 1, &V.X);
  V = VecSet(AP6_Anim.MatrView.A[3][0],
             AP6_Anim.MatrView.A[3][1],
             AP6_Anim.MatrView.A[3][2]);
  loc = glGetUniformLocation(AP6_ShaderProg, "ViewPos");
  if (loc != -1)
    glUniform3fv(loc, 1, &V.X);


  loc = glGetUniformLocation(AP6_ShaderProg, "NumOfParts");
  if (loc != -1)
    glUniform1f(loc, G->NumOfPrims);
  for (i = 0; i < G->NumOfPrims; i++)
  {
    INT mtl = G->Prims[i].Mtl;

    loc = glGetUniformLocation(AP6_ShaderProg, "PartNo");
    if (loc != -1)
      glUniform1f(loc, i);

    /* подготавливаем материал */
    if (G->Mtls != NULL && mtl >= 0 && mtl < G->NumOfMtls)
    {
      INT loc;

      if (G->Mtls[mtl].TexNo == 0 && G->Mtls[mtl].MapD[0] != 0)
      {
        INT j;
        IMAGE Img;
                       
        ImageLoad(&Img, G->Mtls[mtl].MapD);

        /* получаем свободный номер текстуры */
        glGenTextures(1, &G->Mtls[mtl].TexNo);
        /* делаем ее активной */
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        for (j = 0; j < Img.W * Img.H; j++)
          Img.Bits[j] |= 0xFF000000;
        /* отправляем картинку в видеопамять */
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);

        /* Параметры вывода */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      /* передаем параметры */
      if (G->Mtls[mtl].TexNo != 0)
      {
        loc = glGetUniformLocation(AP6_ShaderProg, "IsTextureUsed");
        if (loc != -1)
          glUniform1f(loc, 1);
        loc = glGetUniformLocation(AP6_ShaderProg, "DrawTexture");
        if (loc != -1)
          glUniform1i(loc, 0);
        //glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
      }
      else
      {
        loc = glGetUniformLocation(AP6_ShaderProg, "IsTextureUsed");
        if (loc != -1)
          glUniform1f(loc, 0);
      }
      loc = glGetUniformLocation(AP6_ShaderProg, "Ka");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ka.X);
      loc = glGetUniformLocation(AP6_ShaderProg, "Kd");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Kd.X);
      loc = glGetUniformLocation(AP6_ShaderProg, "Ks");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ks.X);
      loc = glGetUniformLocation(AP6_ShaderProg, "Phong");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Phong);
      loc = glGetUniformLocation(AP6_ShaderProg, "Trans");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Trans);
    }
    loc = glGetUniformLocation(AP6_ShaderProg, "PartNo");
    if (loc != -1)
      glUniform1f(loc, i);
    AP6_PrimDraw(G->Prims + i);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  /* восстанавливаем шейдера */
  if (G->ProgId != 0)
    AP6_ShaderProg = SaveProgId;
} /* End of 'AP6_GeomDraw' function */

/* Функция преобразования вершин.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       ap6GEOM *G;
 *   - матрица преобразования:
 *       MATR M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AP6_GeomTransform( ap6GEOM *G, MATR M )
{
  INT i, j;
  MATR InvM = MatrTranspose(MatrInverse(M));

  InvM.A[3][0] = InvM.A[3][1] = InvM.A[3][2] = 0;

  for (i = 0; i < G->NumOfPrims; i++)
    for (j = 0; j < G->Prims[i].NumOfV; j++)
    {
      G->Prims[i].V[j].P = VecMulMatr(G->Prims[i].V[j].P, M);
      G->Prims[i].V[j].N = VecMulMatr(G->Prims[i].V[j].N, InvM);
    }
} /* End of 'AP6_GeomTransform' function */

/* END OF 'GEOMHAND.C' FILE */
