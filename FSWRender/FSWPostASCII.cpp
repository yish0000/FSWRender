/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPostASCII.cpp
 *  Desc:   本文件实现了一个字符渲染效果。
 *  Author: Yish
 *  Date:   2013/5/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FSWPostASCII.h"


/** 构造函数
*/
FSWPostASCII::FSWPostASCII(FSWSurface* pSurface)
	: FSWPostEffect(POST_ASCII, pSurface)
{
}

/** 析构函数
*/
FSWPostASCII::~FSWPostASCII()
{
}

/** 更新后期特效
*/
void FSWPostASCII::ApplyEffect()
{
}