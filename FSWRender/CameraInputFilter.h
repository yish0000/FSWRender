/*
 * ------------------------------------------------------------------------
 *  Name:   CameraInputFilter.h
 *  Desc:   本文件定义了一个控制相机的输入过滤器。
 *  Author: Yish
 *  Date:   2012/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __CAMERAINPUTFILTER_H__
#define __CAMERAINPUTFILTER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "InputFilter.h"

///////////////////////////////////////////////////////////////////////////


class FSWCamera;

/** 相机控制
*/
class CameraInputFilter : public InputFilter
{
	enum
	{
		LKEY_NONE,			// 无用，此值需保留
		LKEY_MOVE_LEFT,		// 左移
		LKEY_MOVE_RIGHT,	// 右移
		LKEY_MOVE_UP,		// 上移
		LKEY_MOVE_DOWN,		// 下移
	};

public:
	CameraInputFilter(InputManager* pManager, FSWCamera* pCamera);

	// 处理输入
	bool TranslateInput();

	// 获取相机指针
	FSWCamera* GetCamera() const { return m_pCamera; }
	// 设置相机指针
	void SetCamera(FSWCamera* pCamera) { m_pCamera = pCamera; }

protected:
	FSWCamera* m_pCamera;
	bool m_bTurnCamera;
	POINT m_ptBeginPoint;
	POINT m_ptLastPoint;
	bool m_bShowCursor;

	// 响应鼠标左键按下
	void OnLButtonDown( int x,int y,DWORD dwFlags );
	// 响应鼠标左键抬起
	void OnLButtonUp( int x,int y,DWORD dwFlags );
	// 响应鼠标双击
	void OnLButtonDblClk( int x,int y,DWORD dwFlags );
	// 响应鼠标右键按下
	void OnRButtonDown( int x,int y,DWORD dwFlags );
	// 响应鼠标右键抬起
	void OnRButtonUp( int x,int y,DWORD dwFlags );
	// 响应鼠标右键双击
	void OnRButtonDblClk( int x,int y,DWORD dwFlags );
	// 响应鼠标移动
	void OnMouseMove( int x,int y,DWORD dwFlags );
	// 响应鼠标滚轮
	void OnMouseWheel( int x,int y,int iDelta,DWORD dwFlags );

	// 响应按住键盘的移动
	void OnKeyboardMove();

	// 显示/隐藏鼠标
	void ShowCursor( bool bShow );
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __CAMERAINPUTFILTER_H__