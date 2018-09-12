/*
* ------------------------------------------------------------------------
*  Name:   FSWPostInvert.h
*  Desc:   ���ļ�������һ����ת��ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTINVERT_H__
#define __FSW_POSTINVERT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** ��תЧ��
*/
class FSWPostInvert : public FSWPostEffect
{
public:
	FSWPostInvert(FSWSurface* pSurface);
	virtual ~FSWPostInvert();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTINVERT_H__