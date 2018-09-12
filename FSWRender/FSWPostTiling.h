/*
* ------------------------------------------------------------------------
*  Name:   FSWPostTiling.h
*  Desc:   本文件定义了一个块状渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTTILING_H__
#define __FSW_POSTTILING_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** 块状渲染效果
*/
class FSWPostTiling : public FSWPostEffect
{
public:
	FSWPostTiling(FSWSurface* pSurface);
	virtual ~FSWPostTiling();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTTILING_H__