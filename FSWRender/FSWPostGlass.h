/*
* ------------------------------------------------------------------------
*  Name:   FSWPostGlass.h
*  Desc:   ���ļ�������һ��������ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTGLASS_H__
#define __FSW_POSTGLASS_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** ����Ч��
*/
class FSWPostGlass : public FSWPostEffect
{
public:
	FSWPostGlass(FSWSurface* pSurface);
	virtual ~FSWPostGlass();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTGLASS_H__