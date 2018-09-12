/*
* ------------------------------------------------------------------------
*  Name:   FSWPostMotionBlur.cpp
*  Desc:   本文件实现了一个运动模糊渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostMotionBlur.h"


/** 构造函数
*/
FSWPostMotionBlur::FSWPostMotionBlur(FSWSurface* pSurface)
	: FSWPostEffect(POST_MOTIONBLUR, pSurface)
{
}

/** 析构函数
*/
FSWPostMotionBlur::~FSWPostMotionBlur()
{
}

/** 更新后期特效
*/
void FSWPostMotionBlur::ApplyEffect()
{
}