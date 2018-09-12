/*
 * ------------------------------------------------------------------------
 *  Name:   SimpleSceneListener.h
 *  Desc:   ���ļ�������һ�������������ڲ���һЩ�򵥳�����
 *  Author: Yish
 *  Date:   2013/4/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __SIMPLESCENELISTENER_H__
#define __SIMPLESCENELISTENER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FrameListener.h"

///////////////////////////////////////////////////////////////////////////


/** ���Լ򵥳���
*/
class SimpleSceneListener : public FrameListener
{
public:
	SimpleSceneListener(FSWDevice* pDevice);
	~SimpleSceneListener();

	void Tick(float fElapsed);
	void Render();

protected:
	FSWTexture* m_pTexture;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __SIMPLESCENELISTENER_H__