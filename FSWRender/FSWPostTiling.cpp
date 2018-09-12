/*
* ------------------------------------------------------------------------
*  Name:   FSWPostTiling.cpp
*  Desc:   本文件实现了一个块状渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostTiling.h"


/** 构造函数
*/
FSWPostTiling::FSWPostTiling(FSWSurface* pSurface)
	: FSWPostEffect(POST_TILING, pSurface)
{
}

/** 析构函数
*/
FSWPostTiling::~FSWPostTiling()
{
}

/** 更新后期特效
*/
void FSWPostTiling::ApplyEffect()
{
}