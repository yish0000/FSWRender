/*
* ------------------------------------------------------------------------
*  Name:   FSWPostOldTV.h
*  Desc:   本文件定义了一个老旧电视渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTOLDTV_H__
#define __FSW_POSTOLDTV_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** 老旧电视效果
*/
class FSWPostOldTV : public FSWPostEffect
{
public:
	FSWPostOldTV(FSWSurface* pSurface);
	virtual ~FSWPostOldTV();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTOLDTV_H__