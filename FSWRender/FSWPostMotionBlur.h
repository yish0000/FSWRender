/*
* ------------------------------------------------------------------------
*  Name:   FSWPostMotionBlur.h
*  Desc:   ���ļ�������һ���˶�ģ����ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTMOTIONBLUR_H__
#define __FSW_POSTMOTIONBLUR_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** �˶�ģ��Ч��
*/
class FSWPostMotionBlur : public FSWPostEffect
{
public:
	FSWPostMotionBlur(FSWSurface* pSurface);
	virtual ~FSWPostMotionBlur();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTMOTIONBLUR_H__