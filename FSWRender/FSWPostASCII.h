/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPostASCII.h
 *  Desc:   本文件定义了一个字符后期特效。
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


/** 字符渲染效果
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