/*
* ------------------------------------------------------------------------
*  Name:   FSWPostInvert.h
*  Desc:   本文件定义了一个反转渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTINVERT_H__
#define __FSW_POSTINVERT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** 反转效果
*/
class FSWPostInvert : public FSWPostEffect
{
public:
	FSWPostInvert(FSWSurface* pSurface);
	virtual ~FSWPostInvert();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTINVERT_H__