/* FILENAME: UNITS.H
 * PROGRAMMER: AP6
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "anim.h"

/* Функция создания объекта анимации */
ap6UNIT * AP6_CowUnitCreate( VOID ); 

/* Функция создания информационного объекта анимации */ 
ap6UNIT * AP6_InfoUnitCreate( VOID );               

ap6UNIT * AP6_ClockUnitCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
