/*
* ------------------------------------------------------------------------
*  Name:   FSWPostTiling.h
*  Desc:   ���ļ�������һ����״��ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTTILING_H__
#define __FSW_POSTTILING_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** ��״��ȾЧ��
*/
class FSWPostTiling : public FSWPostEffect
{
public:
	FSWPostTiling(FSWSurface* pSurface);
	virtual ~FSWPostTiling();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTTILING_H__