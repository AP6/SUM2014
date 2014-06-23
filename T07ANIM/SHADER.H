/* FILENAME: SHADER.H
 * PROGRAMMER: SS3
 * PURPOSE: Shader handle declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

/* Функция инициализации шейдерной программы */
UINT SS3_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* Функция удаления шейдерной программы */
VOID SS3_ShadProgClose( UINT ProgId );

#endif