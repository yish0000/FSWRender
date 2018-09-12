/*
* ------------------------------------------------------------------------
*  Name:   FSWPostInvert.cpp
*  Desc:   本文件实现了一个反转渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostInvert.h"


/** 构造函数
*/
FSWPostInvert::FSWPostInvert(FSWSurface* pSurface)
	: FSWPostEffect(POST_INVERT, pSurface)
{
}

/** 析构函数
*/
FSWPostInvert::~FSWPostInvert()
{
}

/** 更新后期特效
*/
void FSWPostInvert::ApplyEffect()
{
}