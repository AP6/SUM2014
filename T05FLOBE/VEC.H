/* AP6, 06.06.2014 */


#ifndef __VEC_H_
#define __VEC_H_

/* базовый вещественный тип */
typedef double DBL;

/* тип для вектора в простанстве */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* тип для матрицы - массив в структуре */
typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

extern MATR UnitMatrix;

/* */
__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* */

/* */
__inline DBL VecLen( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len != 0 && len != 1)
    return sqrt(len);
  return len;
} /* */

/* */
__inline DBL VecLen2( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len != 0 && len != 1)
    return len;
  return len * len;
} /* */

/* */
__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* */

/* */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,  
    V1.Z * V2.X - V1.X * V2.Z,
    V1.X * V2.Y - V1.Y * V2.X);
} /* */

/* */
__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* */

/* */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X,
                V1.Y + V2.Y,
                V1.Z + V2.Z);
} /* */

/* */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X,
                V1.Y - V2.Y,
                V1.Z - V2.Z);
} /* */

/* */
__inline VEC VecMulNum( VEC V, DBL N )
{
  V.X = V.X * N;
  V.Y = V.Y * N;
  return V;
} /* */

/* */
__inline VEC VecDivNum( VEC V, DBL N )
{
  V.X = V.X / N;
  V.Y = V.Y / N;
  return V;
} /* */

/* */
__inline VEC VecNormalize( VEC V )
{
  DBL len = VecDotVec(V, V);
  if (len != 0 && len != 1)
    len = sqrt(len);
  V.X /= len, V.Y /= len, V.Z /= len;
  return V;
} /* */

/* */
MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR A =
  {
    {
      { 1,  0,  0, 0},
      { 0,  1,  0, 0},
      { 0,  0,  1, 0},
      {Dx, Dy, Dz, 1},
    }
  };
   return A;
} /* */

/* */
MATR MatrIdenity( VOID )
{
  return UnitMatrix;
} /* */


/* */
__inline MATR MatrScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR m = UnitMatrix;

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;                         
  return m;
} /* */

/* */
__inline MATR MatrRotateX( DBL AngleInDegree )
{
    DBL a, si, co;
  MATR m;

  a = D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);
  m.A[0][0] = 1;
  m.A[0][1] = 0;
  m.A[0][2] = 0;
  m.A[0][3] = 0;
  m.A[1][0] = 0;
  m.A[1][1] = co;
  m.A[1][2] = si;
  m.A[1][3] = 0;
  m.A[2][0] = 0;
  m.A[2][1] = -si;
  m.A[2][2] = co;
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
} /* */

/* */
__inline MATR MatrRotateY( DBL AngleInDegree )
{
    DBL a, si, co;
  MATR m;

  a = D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);
  m.A[0][0] = co;
  m.A[0][1] = 0;
  m.A[0][2] = -si;
  m.A[0][3] = 0;
  m.A[1][0] = 0;
  m.A[1][1] = 1;
  m.A[1][2] = 0;
  m.A[1][3] = 0;
  m.A[2][0] = si;
  m.A[2][1] = 0;
  m.A[2][2] = co;
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;                    
} /* */

/* */
__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  DBL a, si, co;
  MATR m;

  a = D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);
  m.A[0][0] = co;
  m.A[0][1] = si;
  m.A[0][2] = 0;
  m.A[0][3] = 0;
  m.A[1][0] = -si;
  m.A[1][1] = co;
  m.A[1][2] = 0;
  m.A[1][3] = 0;
  m.A[2][0] = 0;
  m.A[2][1] = 0;
  m.A[2][2] = 1;
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
} /* */

/* */
__inline MATR MatrRotate( DBL AngleInDegree,
                 DBL X, DBL Y, DBL Z )
{
  DBL a, si, co, len;
  MATR m;

  a = D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;
  X *= si;
  Y *= si;
  Z *= si;
  m.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  m.A[0][1] = 2 * X * Y - 2 * co * Z;
  m.A[0][2] = 2 * co * Y + 2 * X * Z;
  m.A[0][3] = 0;
  m.A[1][0] = 2 * X * Y + 2 * co * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z * Z);
  m.A[1][2] = 2 * Y * Z - 2 * co * X;
  m.A[1][3] = 0;
  m.A[2][0] = 2 * X * Z - 2 * co * Y;
  m.A[2][1] = 2 * co * X + 2 * Y * Z;
  m.A[2][2] = 1 - 2 * (X * X + Y * Y);
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
} /* */

/* */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR m;
  int i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (m.A[i][j] = 0, k = 0; k < 4; k++)
        m.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return m;
} /* */

/* */
__inline MATR MatrTranspose( MATR M )
{
  MATR m;

  m.A[0][0] = M.A[0][0];
  m.A[0][1] = M.A[1][0];
  m.A[0][2] = M.A[2][0];
  m.A[0][3] = M.A[3][0];
  m.A[1][0] = M.A[0][1];
  m.A[1][1] = M.A[1][1];
  m.A[1][2] = M.A[2][1];
  m.A[1][3] = M.A[3][1];
  m.A[2][0] = M.A[0][2];
  m.A[2][1] = M.A[1][2];
  m.A[2][2] = M.A[2][2];
  m.A[2][3] = M.A[3][2];
  m.A[3][0] = M.A[0][3];
  m.A[3][1] = M.A[1][3];
  m.A[3][2] = M.A[2][3];
  m.A[3][3] = M.A[3][3];
} /* */

/* */
__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 +
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* */

/* */
__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
} /* */

/* */
__inline MATR MatrInverse( MATR M )
{
  int i, j;
  MATR r;
  DBL det = MatrDeterm(M);
  int perm[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };

  if (det == 0)
    return UnitMatrix;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[j][i] =
        MatrDeterm3x3(
          M.A[perm[i][0]][perm[j][0]],
          M.A[perm[i][0]][perm[j][1]],
          M.A[perm[i][0]][perm[j][2]],
          M.A[perm[i][1]][perm[j][0]],
          M.A[perm[i][1]][perm[j][1]],
          M.A[perm[i][1]][perm[j][2]],
          M.A[perm[i][2]][perm[j][0]],
          M.A[perm[i][2]][perm[j][1]],
          M.A[perm[i][2]][perm[j][2]]) / det;
  return r;
} /*  */

VEC PointTransform( VEC V, MATR M )
{
  return VecSet( V.X * M.A[0][0] + V.Y * M.A[1][0]  + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1]  + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2]  + V.Z * M.A[2][2] + M.A[3][2]);

}

VEC VectorTransform( VEC V, MATR M )
{
  return VecSet( V.X * M.A[0][0] + V.Y * M.A[1][0]  + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1]  + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2]  + V.Z * M.A[2][2]);
}



#endif /* __VEC_H_ */