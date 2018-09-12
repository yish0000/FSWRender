/*
* ------------------------------------------------------------------------
*  Name:   FSWPostNightVision.cpp
*  Desc:   本文件实现了一个夜视渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostNightVision.h"


/** 构造函数
*/
FSWPostNightVision::FSWPostNightVision(FSWSurface* pSurface)
	: FSWPostEffect(POST_NIGHTVISION, pSurface)
{
}

/** 析构函数
*/
FSWPostNightVision::~FSWPostNightVision()
{
}

/** 更新后期特效
*/
void FSWPostNightVision::ApplyEffect()
{
}