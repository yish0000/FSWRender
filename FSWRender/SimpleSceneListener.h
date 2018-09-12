/*
 * ------------------------------------------------------------------------
 *  Name:   SimpleSceneListener.h
 *  Desc:   本文件定义了一个监听器，用于测试一些简单场景。
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


/** 测试简单场景
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