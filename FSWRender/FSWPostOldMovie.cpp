/*
* ------------------------------------------------------------------------
*  Name:   FSWPostOldMovie.cpp
*  Desc:   本文件实现了一个老旧电影渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostOldMovie.h"


/** 构造函数
*/
FSWPostOldMovie::FSWPostOldMovie(FSWSurface* pSurface)
	: FSWPostEffect(POST_OLDMOVIE, pSurface)
{
}

/** 析构函数
*/
FSWPostOldMovie::~FSWPostOldMovie()
{
}

/** 更新后期特效
*/
void FSWPostOldMovie::ApplyEffect()
{
}