/*
* ------------------------------------------------------------------------
*  Name:   FSWPostOldTV.h
*  Desc:   ���ļ�������һ���Ͼɵ�����ȾЧ����
*  Author: Yish
*  Date:   2013/5/9
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __FSW_POSTOLDTV_H__
#define __FSW_POSTOLDTV_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** �Ͼɵ���Ч��
*/
class FSWPostOldTV : public FSWPostEffect
{
public:
	FSWPostOldTV(FSWSurface* pSurface);
	virtual ~FSWPostOldTV();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTOLDTV_H__