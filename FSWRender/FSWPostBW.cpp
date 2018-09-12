/*
* ------------------------------------------------------------------------
*  Name:   FSWPostBW.cpp
*  Desc:   本文件实现了一个字符渲染效果。
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostBW.h"


/** 构造函数
*/
FSWPostBW::FSWPostBW(FSWSurface* pSurface)
	: FSWPostEffect(POST_BW, pSurface)
{
}

/** 析构函数
*/
FSWPostBW::~FSWPostBW()
{
}

/** 更新后期特效
*/
void FSWPostBW::ApplyEffect()
{
	DWORD* pSrcBuffer = (DWORD*)m_pPrevSurface->GetBufferData();
	DWORD* pSrcEnd = pSrcBuffer + m_pPrevSurface->GetWidth() * m_pPrevSurface->GetHeight();
	DWORD* pDestBuffer = (DWORD*)m_pNextSurface->GetBufferData();

	while( pSrcBuffer < pSrcEnd )
	{
		MakePixel(pSrcBuffer++, pDestBuffer++);
	}
}

// 处理像素
void FSWPostBW::MakePixel(DWORD* pSrcPixel, DWORD* pDestPixel)
{
	ColorF destColor;
	ColorF srcColor(*pSrcPixel);
	float fGray = srcColor.fR * 0.4f + srcColor.fG * 0.59f + srcColor.fB * 0.11f;
	destColor.fR = destColor.fG = destColor.fB = fGray;
	destColor.fA = srcColor.fA;
	*pDestPixel = destColor.ToColor().val;
}