/* FILENAME: SHADER.H
 * PROGRAMMER: SS3
 * PURPOSE: Shader handle declaration module.
 * LAST UPDATE: 23.06.2014
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

/* ������� ������������� ��������� ��������� */
UINT AP6_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* ������� �������� ��������� ��������� */
VOID AP6_ShadProgClose( UINT ProgId );

#endif