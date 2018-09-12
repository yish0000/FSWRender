/*
* ------------------------------------------------------------------------
*  Name:   FSWPostDither.h
*  Desc:   ���ļ�������һ���ڵ���ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTDITHER_H__
#define __FSW_POSTDITHER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** �ڵ���ȾЧ��
*/
class FSWPostDither : public FSWPostEffect
{
public:
	FSWPostDither(FSWSurface* pSurface);
	virtual ~FSWPostDither();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTDITHER_H__