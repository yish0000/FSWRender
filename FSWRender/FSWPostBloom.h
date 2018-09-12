/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPostBloom.h
 *  Desc:   ���ļ�������һ��BLOOMЧ����
 *  Author: Yish
 *  Date:   2013/5/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_POSTBLOOM_H__
#define __FSW_POSTBLOOM_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** BLOOM�Թ�Ч��
*/
class FSWPostBloom : public FSWPostEffect
{
public:
	FSWPostBloom(FSWSurface* pSurface);
	virtual ~FSWPostBloom();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTBLOOM_H__