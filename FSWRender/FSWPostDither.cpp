/*
* ------------------------------------------------------------------------
*  Name:   FSWPostDither.cpp
*  Desc:   ���ļ�ʵ����һ���ڵ���ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostDither.h"


/** ���캯��
*/
FSWPostDither::FSWPostDither(FSWSurface* pSurface)
	: FSWPostEffect(POST_DITHER, pSurface)
{
}

/** ��������
*/
FSWPostDither::~FSWPostDither()
{
}

/** ���º�����Ч
*/
void FSWPostDither::ApplyEffect()
{
}