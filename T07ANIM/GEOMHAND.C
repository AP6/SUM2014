/* FILENAME: GEOMHAND.C
 * PROGRAMMER: AP6
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 23.06.2014
 */

#include <stdlib.h>

#include "anim.h"

/* ������� ���������� ��������� � �������.
 * ���������:
 *   - �������������� ������:
 *       ap6GEOM *G;
 *   - ����������� ��������:
 *       ap6MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� ��� -1
 *         ��� ������.
 */
INT AP6_GeomAddMaterial( ap6GEOM *G, ap6MATERIAL *Mtl )
{
  INT i;
  ap6MATERIAL *M;

  /* ���� �������� � ���������� */
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
    /* �������� ������ ������ */
    memcpy(M, G->Mtls, sizeof(ap6MATERIAL) * G->NumOfMtls);
    /* ����������� ������ */
    free(G->Mtls);
  }
  /* ��������� */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'AP6_GeomAddMaterial' function */

/* ������� ���������� ��������� � �������.
 * ���������:
 *   - �������������� ������:
 *       ap6GEOM *G;
 *   - ����������� ��������:
 *       ap6PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� ��� -1
 *         ��� ������.
 */
INT AP6_GeomAddPrim( ap6GEOM *G, ap6PRIM *Prim )
{
  ap6PRIM *P;

  if ((P = malloc(sizeof(ap6PRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* �������� ������ ������ */
    memcpy(P, G->Prims, sizeof(ap6PRIM) * G->NumOfPrims);
    /* ����������� ������ */
    free(G->Prims);
  }
  /* ��������� */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'AP6_GeomAddPrim' function */

/* ������� ������������.
 * ���������:
 *   - �������������� ������:
 *       ap6GEOM *G;
 * ������������ ��������: ���.
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

/* ������� ���������.
 * ���������:
 *   - �������������� ������:
 *       ap6GEOM *G;
 * ������������ ��������: ���.
 */
VOID AP6_GeomDraw( ap6GEOM *G )
{
  INT i, loc;
  MATR WVP, MatrWorldInvTrans = {{{0}}};
  VEC V;

  /* ��������� ������� �������������� */
  WVP = MatrMulMatr(AP6_Anim.MatrWorld,
    MatrMulMatr(AP6_Anim.MatrView, AP6_Anim.MatrProjection));
  MatrWorldInvTrans = MatrTranspose(MatrInverse(AP6_Anim.MatrWorld));

  /* ����� ��������� �������� ������ ���������� */
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

    /* �������������� �������� */
    if (G->Mtls != NULL && mtl >= 0 && mtl < G->NumOfMtls)
    {
      INT loc;

      if (G->Mtls[mtl].TexNo == 0 && G->Mtls[mtl].MapD[0] != 0)
      {
        INT j;
        IMAGE Img;
                       
        ImageLoad(&Img, G->Mtls[mtl].MapD);

        /* �������� ��������� ����� �������� */
        glGenTextures(1, &G->Mtls[mtl].TexNo);
        /* ������ �� �������� */
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        for (j = 0; j < Img.W * Img.H; j++)
          Img.Bits[j] |= 0xFF000000;
        /* ���������� �������� � ����������� */
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);

        /* ��������� ������ */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      /* �������� ��������� */
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
} /* End of 'AP6_GeomDraw' function */

/* ������� �������������� ������.
 * ���������:
 *   - �������������� ������:
 *       ap6GEOM *G;
 *   - ������� ��������������:
 *       MATR M;
 * ������������ ��������: ���.
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
