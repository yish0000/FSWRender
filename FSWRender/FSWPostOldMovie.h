/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPostOldMovie.h
 *  Desc:   ���ļ�������һ���Ͼɵ�ӰЧ����
 *  Author: Yish
 *  Date:   2013/5/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_POSTOLDMOVIE_H__
#define __FSW_POSTOLDMOVIE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** �Ͼɵ�ӰЧ��
*/
class FSWPostOldMovie : public FSWPostEffect
{
public:
	FSWPostOldMovie(FSWSurface* pSurface);
	virtual ~FSWPostOldMovie();

	void ApplyEffect();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FSW_POSTOLDMOVIE_H__