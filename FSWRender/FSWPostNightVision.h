/*
* ------------------------------------------------------------------------
*  Name:   FSWPostNightVision.h
*  Desc:   ���ļ�������һ��ҹ����ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTNIGHTVISION_H__
#define __FSW_POSTNIGHTVISION_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** ҹ��Ч��
*/
class FSWPostNightVision : public FSWPostEffect
{
public:
	FSWPostNightVision(FSWSurface* pSurface);
	virtual ~FSWPostNightVision();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTNIGHTVISION_H__