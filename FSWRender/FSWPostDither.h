/*
* ------------------------------------------------------------------------
*  Name:   FSWPostDither.h
*  Desc:   本文件定义了一个黑点渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTDITHER_H__
#define __FSW_POSTDITHER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** 黑点渲染效果
*/
class FSWPostDither : public FSWPostEffect
{
public:
	FSWPostDither(FSWSurface* pSurface);
	virtual ~FSWPostDither();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTDITHER_H__