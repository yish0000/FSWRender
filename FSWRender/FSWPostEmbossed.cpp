/*
* ------------------------------------------------------------------------
*  Name:   FSWPostEmbossed.cpp
*  Desc:   本文件实现了一个浮雕渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostEmbossed.h"


/** 构造函数
*/
FSWPostEmbossed::FSWPostEmbossed(FSWSurface* pSurface)
	: FSWPostEffect(POST_EMBOSSED, pSurface)
{
}

/** 析构函数
*/
FSWPostEmbossed::~FSWPostEmbossed()
{
}

/** 更新后期特效
*/
void FSWPostEmbossed::ApplyEffect()
{
}