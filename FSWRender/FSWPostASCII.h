/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPostASCII.h
 *  Desc:   ���ļ�������һ���ַ�������Ч��
 *  Author: Yish
 *  Date:   2013/5/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_POSTASCII_H__
#define __FSW_POSTASCII_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** �ַ���ȾЧ��
*/
class FSWPostASCII : public FSWPostEffect
{
public:
	FSWPostASCII(FSWSurface* pSurface);
	virtual ~FSWPostASCII();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTASCII_H__