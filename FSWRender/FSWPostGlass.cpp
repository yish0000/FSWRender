/*
* ------------------------------------------------------------------------
*  Name:   FSWPostGlass.cpp
*  Desc:   本文件实现了一个玻璃渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostGlass.h"


/** 构造函数
*/
FSWPostGlass::FSWPostGlass(FSWSurface* pSurface)
	: FSWPostEffect(POST_GLASS, pSurface)
{
}

/** 析构函数
*/
FSWPostGlass::~FSWPostGlass()
{
}

/** 更新后期特效
*/
void FSWPostGlass::ApplyEffect()
{
}