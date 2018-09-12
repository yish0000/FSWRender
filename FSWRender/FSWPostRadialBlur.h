/*
* ------------------------------------------------------------------------
*  Name:   FSWPostRadialBlur.h
*  Desc:   本文件定义了一个径向模糊渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTRADIALBLUR_H__
#define __FSW_POSTRADIALBLUR_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** 径向模糊渲染效果
*/
class FSWPostRadialBlur : public FSWPostEffect
{
public:
	FSWPostRadialBlur(FSWSurface* pSurface);
	virtual ~FSWPostRadialBlur();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTRADIALBLUR_H__