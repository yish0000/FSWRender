/*
* ------------------------------------------------------------------------
*  Name:   FSWPostMotionBlur.cpp
*  Desc:   ���ļ�ʵ����һ���˶�ģ����ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#include "FSWPostMotionBlur.h"


/** ���캯��
*/
FSWPostMotionBlur::FSWPostMotionBlur(FSWSurface* pSurface)
	: FSWPostEffect(POST_MOTIONBLUR, pSurface)
{
}

/** ��������
*/
FSWPostMotionBlur::~FSWPostMotionBlur()
{
}

/** ���º�����Ч
*/
void FSWPostMotionBlur::ApplyEffect()
{
}