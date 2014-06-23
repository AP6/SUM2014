/* FILENAME: ANIM.H
 * PROGRAMMER: AP6
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Максимальное количество элементов анимации */
#define AP6_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagap6UNIT ap6UNIT;

/* Структура хранения контекста анимации */
typedef struct tagap6ANIM
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст вывода */

  /* Массив элементов анимации и их количество */
  ap6UNIT *Units[AP6_MAX_UNITS]; 
  INT NumOfUnits;

  /* Подсистема синхронизации */
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS,             /* количество кадров в секунду */
    JX, JY, JZ, JU, JR;

  DWORD JPOV;

  BYTE
    JButsOld[32],
    JButs[32],
    JButsClick[32];

  BOOL
    IsPause;         /* флаг паузы */

  /* Подсистема ввода */
  BYTE Keys[256],
    KeysClick[256];     /* Сотояние клавиш клавиатуры и мыши */
} ap6ANIM;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - указатель на функцию инициализации
 *     VOID (*Init)( ap6UNIT *Unit, ap6ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( ap6UNIT *Unit, ap6ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( ap6UNIT *Unit, ap6ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( ap6UNIT *Unit, ap6ANIM *Ani );
 */
#define AP6_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ap6UNIT *Unit, ap6ANIM *Ani );       \
  VOID (*Close)( ap6UNIT *Unit, ap6ANIM *Ani );      \
  VOID (*Response)( ap6UNIT *Unit, ap6ANIM *Ani );   \
  VOID (*Render)( ap6UNIT *Unit, ap6ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagap6UNIT
{
  AP6_UNIT_BASE_FIELDS; /* Базовые поля */
};

/***
 * Функции управления анимацией
 ***/

/* Функция инициализации анимации */
VOID AP6_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID AP6_AnimClose( VOID );

/* Функция обработки изменения размера области вывода */
VOID AP6_AnimResize( INT W, INT H );

/* Функция построения кадра анимации */
VOID AP6_AnimRender( VOID );

/* Функция вывода кадра анимации */
VOID AP6_AnimCopyFrame( VOID );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов */
VOID AP6_AnimFlipFullScreen( VOID );

/* Функция установки паузы анимации */
VOID AP6_AnimSetPause( BOOL NewPauseFlag );

/***
 * Функции обработки объектов анимации
 ***/

/* Функция добавления в систему объекта анимации */
VOID AP6_AnimAddUnit( ap6UNIT *Unit );

/* Функция создания объекта анимации */
ap6UNIT * AP6_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
