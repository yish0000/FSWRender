/*
* ------------------------------------------------------------------------
*  Name:   FSWPostBW.cpp
*  Desc:   ���ļ�ʵ����һ���ַ���ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostBW.h"


/** ���캯��
*/
FSWPostBW::FSWPostBW(FSWSurface* pSurface)
	: FSWPostEffect(POST_BW, pSurface)
{
}

/** ��������
*/
FSWPostBW::~FSWPostBW()
{
}

/** ���º�����Ч
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

// ��������
void FSWPostBW::MakePixel(DWORD* pSrcPixel, DWORD* pDestPixel)
{
	ColorF destColor;
	ColorF srcColor(*pSrcPixel);
	float fGray = srcColor.fR * 0.4f + srcColor.fG * 0.59f + srcColor.fB * 0.11f;
	destColor.fR = destColor.fG = destColor.fB = fGray;
	destColor.fA = srcColor.fA;
	*pDestPixel = destColor.ToColor().val;
}