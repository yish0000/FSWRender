/*
* ------------------------------------------------------------------------
*  Name:   FSWPostEmbossed.h
*  Desc:   ���ļ�������һ��������ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTEMBOSSED_H__
#define __FSW_POSTEMBOSSED_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** ����Ч��
*/
class FSWPostEmbossed : public FSWPostEffect
{
public:
	FSWPostEmbossed(FSWSurface* pSurface);
	virtual ~FSWPostEmbossed();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTEMBOSSED_H__