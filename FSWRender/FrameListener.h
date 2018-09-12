/*
 * ------------------------------------------------------------------------
 *  Name:   FrameListener.h
 *  Desc:   ���ļ�������һ��֡���������ࡣ
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

/** ֡����������
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