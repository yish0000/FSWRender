/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPostBW.h
 *  Desc:   ���ļ�������һ���ڰ���ȾЧ����
 *  Author: Yish
 *  Date:   2013/5/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_POSTBW_H__
#define __FSW_POSTBW_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** �ڰ���ȾЧ��
*/
class FSWPostBW : public FSWPostEffect
{
public:
	FSWPostBW(FSWSurface* pSurface);
	virtual ~FSWPostBW();

	void ApplyEffect();

protected:
	void MakePixel(DWORD* pSrcPixel, DWORD* pDestPixel);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTBW_H__