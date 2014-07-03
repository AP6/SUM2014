/* FILENAME: SHADER.H
 * PROGRAMMER: SS3
 * PURPOSE: Shader handle declaration module.
 * LAST UPDATE: 23.06.2014
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

/* Функция инициализации шейдерной программы */
UINT AP6_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* Функция удаления шейдерной программы */
VOID AP6_ShadProgClose( UINT ProgId );

#endif