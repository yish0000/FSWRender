/*
* ------------------------------------------------------------------------
*  Name:   FSWPostNightVision.h
*  Desc:   本文件定义了一个夜视渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTNIGHTVISION_H__
#define __FSW_POSTNIGHTVISION_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** 夜视效果
*/
class FSWPostNightVision : public FSWPostEffect
{
public:
	FSWPostNightVision(FSWSurface* pSurface);
	virtual ~FSWPostNightVision();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTNIGHTVISION_H__