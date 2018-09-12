/*
* ------------------------------------------------------------------------
*  Name:   FSWPostRadialBlur.cpp
*  Desc:   本文件实现了一个径向模糊渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostRadialBlur.h"


/** 构造函数
*/
FSWPostRadialBlur::FSWPostRadialBlur(FSWSurface* pSurface)
	: FSWPostEffect(POST_RADIALBLUR, pSurface)
{
}

/** 析构函数
*/
FSWPostRadialBlur::~FSWPostRadialBlur()
{
}

/** 更新后期特效
*/
void FSWPostRadialBlur::ApplyEffect()
{
}