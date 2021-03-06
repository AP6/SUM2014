/* FILENAME: ANIM.H
 * PROGRAMMER: AP6
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 23.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"
#include "render.h"

/* ������������ ���������� ��������� �������� */
#define AP6_MAX_UNITS 3000

/* ���������� ���� �������� �������� "������" */
typedef struct tagap6UNIT ap6UNIT;

/* ��������� �������� ��������� �������� */
typedef struct tagap6ANIM
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HDC hDC;            /* �������� ���� ������ */
  HGLRC hRC;          /* �������� ���������� */

  /* ������ ��������� �������� � �� ���������� */
  ap6UNIT *Units[AP6_MAX_UNITS]; 
  INT NumOfUnits;

  /* ���������� ������������� */
  FLT
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS,             /* ���������� ������ � ������� */
    JX, JY, JZ, JU, JR;


    /* ��������� ������������� */
  FLT
    Wp, Hp,    /* ������� ������� ������������� */
    ProjDist,   /* ���������� �� ��������� �������� */
    ProjClip,
    ProjSize;      

  /* ������� */
  MATR
    MatrWorld,        /* ������� �������������� ������� �� */
    MatrView,         /* ������� �������������� ������� �� */
    MatrProjection;   /* ������� �������� */

  DWORD JPOV;

  BYTE
    JButsOld[32],
    JButs[32],
    JButsClick[32];

  BOOL
    IsPause;         /* ���� ����� */

  /* ���������� ����� */
  BYTE Keys[256],
    KeysClick[256];     /* �������� ������ ���������� � ���� */
} ap6ANIM;

extern ap6ANIM AP6_Anim;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( ap6UNIT *Unit, ap6ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( ap6UNIT *Unit, ap6ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( ap6UNIT *Unit, ap6ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( ap6UNIT *Unit, ap6ANIM *Ani );
 */
#define AP6_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ap6UNIT *Unit, ap6ANIM *Ani );       \
  VOID (*Close)( ap6UNIT *Unit, ap6ANIM *Ani );      \
  VOID (*Response)( ap6UNIT *Unit, ap6ANIM *Ani );   \
  VOID (*Render)( ap6UNIT *Unit, ap6ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagap6UNIT
{
  AP6_UNIT_BASE_FIELDS; /* ������� ���� */
};

/***
 * ������� ���������� ���������
 ***/

/* ������� ������������� �������� */
BOOL AP6_AnimInit( HWND hWnd );

/* ������� ��������������� �������� */
VOID AP6_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������ */
VOID AP6_AnimResize( INT W, INT H );

/* ������� ���������� ����� �������� */
VOID AP6_AnimRender( VOID );

/* ������� ������ ����� �������� */
VOID AP6_AnimCopyFrame( VOID );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ��������� */
VOID AP6_AnimFlipFullScreen( VOID );

/* ������� ��������� ����� �������� */
VOID AP6_AnimSetPause( BOOL NewPauseFlag );

/***
 * ������� ��������� �������� ��������
 ***/

/* ������� ���������� � ������� ������� �������� */
VOID AP6_AnimAddUnit( ap6UNIT *Unit );

/* ������� �������� ������� �������� */
ap6UNIT * AP6_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
