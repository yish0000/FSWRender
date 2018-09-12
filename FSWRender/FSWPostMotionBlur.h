/*
* ------------------------------------------------------------------------
*  Name:   FSWPostMotionBlur.h
*  Desc:   本文件定义了一个运动模糊渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTMOTIONBLUR_H__
#define __FSW_POSTMOTIONBLUR_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** 运动模糊效果
*/
class FSWPostMotionBlur : public FSWPostEffect
{
public:
	FSWPostMotionBlur(FSWSurface* pSurface);
	virtual ~FSWPostMotionBlur();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTMOTIONBLUR_H__