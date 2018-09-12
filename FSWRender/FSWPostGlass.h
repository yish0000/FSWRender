/*
* ------------------------------------------------------------------------
*  Name:   FSWPostGlass.h
*  Desc:   本文件定义了一个玻璃渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTGLASS_H__
#define __FSW_POSTGLASS_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** 玻璃效果
*/
class FSWPostGlass : public FSWPostEffect
{
public:
	FSWPostGlass(FSWSurface* pSurface);
	virtual ~FSWPostGlass();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTGLASS_H__