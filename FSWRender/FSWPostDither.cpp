/*
* ------------------------------------------------------------------------
*  Name:   FSWPostDither.cpp
*  Desc:   本文件实现了一个黑点渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostDither.h"


/** 构造函数
*/
FSWPostDither::FSWPostDither(FSWSurface* pSurface)
	: FSWPostEffect(POST_DITHER, pSurface)
{
}

/** 析构函数
*/
FSWPostDither::~FSWPostDither()
{
}

/** 更新后期特效
*/
void FSWPostDither::ApplyEffect()
{
}