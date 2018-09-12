/*
* ------------------------------------------------------------------------
*  Name:   FSWPostRadialBlur.h
*  Desc:   ���ļ�������һ������ģ����ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTRADIALBLUR_H__
#define __FSW_POSTRADIALBLUR_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** ����ģ����ȾЧ��
*/
class FSWPostRadialBlur : public FSWPostEffect
{
public:
	FSWPostRadialBlur(FSWSurface* pSurface);
	virtual ~FSWPostRadialBlur();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTRADIALBLUR_H__