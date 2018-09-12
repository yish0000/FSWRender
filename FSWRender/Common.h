/*
 * ------------------------------------------------------------------------
 *  Name:   Common.h
 *  Desc:   ���ļ��ǳ���Ĺ���ͷ�ļ���
 *  Author: Yish
 *  Date:   2011/7/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __COMMON_H__
#define __COMMON_H__


//#pragma warning(disable:4244)

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

//// HEADERS OF THIS FILE /////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <assert.h>
#include <windows.h>

#include <string>
#include <vector>
#include <algorithm>
#include <map>

///////////////////////////////////////////////////////////////////////////


// ����һЩ��
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p) = NULL; } }

#ifndef ASSERT
#define ASSERT assert
#endif

typedef std::string String;
typedef std::wstring WString;

///////////////////////////////////////////////////////////////////////////
// һЩģ�庯��

template <typename T>
T F_Clamp( T val, T low, T high )
{
    if( val > high ) return high;
    if( val < low ) return low;
    return val;
}

template <typename T>
void F_Clamp2( T& val, T low, T high )
{
    if( val > high ) val = high;
    if( val < low ) val = low;
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __COMMON_H__