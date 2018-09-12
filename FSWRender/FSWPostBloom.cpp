/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPostBloom.cpp
 *  Desc:   本文件实现了一个BLOOM效果。
 *  Author: Yish
 *  Date:   2013/5/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FSWPostBloom.h"


/** 构造函数
*/
FSWPostBloom::FSWPostBloom(FSWSurface* pSurface)
	: FSWPostEffect(POST_BLOOM, pSurface)
{
}

/** 析构函数
*/
FSWPostBloom::~FSWPostBloom()
{
}

/** 更新后期特效
*/
void FSWPostBloom::ApplyEffect()
{
}