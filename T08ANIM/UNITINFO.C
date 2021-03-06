 /* FILENAME: UNITINFO.C
 * PROGRAMMER: AP6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "render.h"
#include "anim.h"

/* ��������� �������� ������� �������� */
typedef struct tagap6UNIT_COW
{
  AP6_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  DBL ShiftX, ShiftY;   /* �������� */
  INT Type;             /* ��� */
  ap6GEOM Info;
  ap6GEOM Land;
  ap6GEOM Water;
  ap6GEOM InchWorm;
} ap6UNIT_COW;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitInit( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  ap6PRIM p;
  ap6MATERIAL mtl;

  AP6_GeomLoad(&Unit->Info, "E:/Models/Rattlesnake/RATTLER.OBJ");  
  strcpy(Unit->Info.ProgName, "snake");

//  AP6_GeomLoad(&Unit->InchWorm, "E:/Models/Inchworm/INCHWORM.OBJ");
//  strcpy(Unit->InchWorm.ProgName, "snake");

  /* �������� ��������� */
  memset(&Unit->Land, 0, sizeof(ap6GEOM));
  strcpy(Unit->Land.ProgName, "land");
//  AP6_PrimCreateHeightField(&p, "hf.bmp", 3.0, 10);
  AP6_PrimCreateHeightField(&p, "hf.bmp", 2.0, 55);


  mtl.Ka = VecSet(0.1, 0.1, 0.1);
  mtl.Kd = VecSet(1, 1, 1);
  mtl.Ks = VecSet(0, 0, 0);
  mtl.Phong = 30;
  mtl.Trans = 1;
  mtl.TexNo = 0;
  mtl.MapD[0] = 0;
  strcpy(mtl.MapD, "map2-hf.bmp");
  strcpy(mtl.MapD, "r.bmp");
  strcpy(mtl.Name, "Height Field Material");
  p.Mtl = AP6_GeomAddMaterial(&Unit->Land, &mtl);

  AP6_GeomAddPrim(&Unit->Land, &p);


  /* �������� ���� */
  memset(&Unit->Water, 0, sizeof(ap6GEOM));
  AP6_DefaultColor = ColorSet(0, 0, 1);
  AP6_GeomLoad(&Unit->Water, "WATER.OBJECT");  
  strcpy(Unit->Water.ProgName, "water");
} /* End of 'InfoUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitClose( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  AP6_GeomFree(&Unit->Info);
} /* End of 'InfoUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitResponse( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  if (Ani->Keys[VK_LMENU] && Ani->KeysClick[VK_RETURN])
    AP6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AP6_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);         
} /* End of 'InfoUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ap6UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       ap6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitRender( ap6UNIT_COW *Unit, ap6ANIM *Ani )
{
  INT i, j;
  static DBL x, y, z, u, h, dist = 1;
  static DBL time;
  
  x += Ani->JX * Ani->GlobalDeltaTime * 30;
  y += Ani->JY * Ani->GlobalDeltaTime * 30;
  z += Ani->JZ * Ani->GlobalDeltaTime * 30;
  u += Ani->JR * Ani->GlobalDeltaTime * 30;
  dist +=  (Ani->Keys[VK_ADD] - Ani->Keys[VK_SUBTRACT]) * Ani->GlobalDeltaTime * 3.0;
  h += ((Ani->JPOV == 1) - (Ani->JPOV == 5)) * 3 * Ani->GlobalDeltaTime;

  if (Ani->KeysClick['R'])
    x = y = z = u = h = 0;

//  Ani->MatrWorld = MatrMulMatr(MatrTranslate(x, h, y), MatrMulMatr(MatrRotateY(z), MatrRotateX(u)));
  Ani->MatrWorld = MatrTranslate(x, h, y);
  Ani->MatrView = MatrViewLookAt(VecMulNum(VecSet(5, 5, 5), dist), VecSet(0, 0, 0), VecSet(0, 1, 0));

  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  for (i = -1; i <= 1; i++)
    for (j = -1; j <= 1; j++)
    {
      INT loc;

      glUseProgram(Unit->Info.ProgId); 
      loc = glGetUniformLocation(Unit->Info.ProgId, "indI");
      if (loc != -1)
        glUniform1f(loc, i);
      loc = glGetUniformLocation(Unit->Info.ProgId, "indJ");
      if (loc != -1)
        glUniform1f(loc, j);

      Ani->MatrWorld = MatrMulMatr(MatrTranslate(i * 2 +  x, 0, j * 3 +  z), MatrMulMatr(MatrRotateY(y), MatrRotateX(u)));;
      AP6_GeomDraw(&Unit->Info);
    }
  AP6_GeomDraw(&Unit->Land);
  AP6_GeomDraw(&Unit->Water);
} /* End of 'InfoUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (ap6UNIT *) ��������� �� ��������� ������ ��������.
 */
ap6UNIT * AP6_InfoUnitCreate( VOID )
{
  ap6UNIT_COW *Unit;

  if ((Unit = (ap6UNIT_COW *)AP6_AnimUnitCreate(sizeof(ap6UNIT_COW))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Close = (VOID *)InfoUnitClose;
  Unit->Response = (VOID *)InfoUnitResponse;
  Unit->Render = (VOID *)InfoUnitRender;
  return (ap6UNIT *)Unit;           
} /* End of 'AP6_InfoUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */