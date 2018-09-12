/*
 * ------------------------------------------------------------------------
 *  Name:   FrameListener.h
 *  Desc:   本文件定义了一个帧监听器基类。
 *  Author: Yish
 *  Date:   2013/5/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FRAMELISTENER_H__
#define __FRAMELISTENER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Common.h"

///////////////////////////////////////////////////////////////////////////


class FSWDevice;

/** 帧监听器基类
*/
class FrameListener
{
public:
	FrameListener( FSWDevice* pDevice )
		: m_pDevice(pDevice)
	{
	}

	virtual void Tick(float fElapsed) = 0;
	virtual void Render() = 0;

protected:
	FSWDevice* m_pDevice;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FRAMELISTENER_H__