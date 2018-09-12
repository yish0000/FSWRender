/*
 * ------------------------------------------------------------------------
 *  Name:   SysInputFilter.h
 *  Desc:   本文件定义了一个系统的输入过滤器。
 *  Author: Yish
 *  Date:   2012/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __SYSINPUTFILTER_H__
#define __SYSINPUTFILTER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "InputFilter.h"

///////////////////////////////////////////////////////////////////////////


/** 系统输入过滤器
*/
class SysInputFilter : public InputFilter
{
	enum
	{
		LKEY_NONE,			// 此值无用，需保留
		LKEY_PRINTSCREEN,	// 截取当前屏幕
		LKEY_FULLSCREEN,	// 切换全屏模式
		LKEY_WIREFRAME,		// 切换线框模式
		LKEY_SHOWFPS,		// 切换显示FPS

		LKEY_SELECTMODE1,	// 选择程序的模式
		LKEY_SELECTMODE2,
		LKEY_SELECTMODE3,
		LKEY_SELECTMODE4,
		LKEY_SELECTMODE5,
		LKEY_SELECTMODE6,

		LKEY_QUALITY_LOW,	// 加载资源的大小
		LKEY_QUALITY_MIDDLE,
		LKEY_QUALITY_HIGH,
	};

public:
	SysInputFilter( InputManager* pManager );

	// 处理输入
	bool TranslateInput();

private:

	// 响应键盘按下
	void OnKeyDown(UINT uMsg, WPARAM wParam, DWORD dwCtrlKeys);

	// 截取屏幕
	void PrintScreen();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __SYSINPUTFILTER_H__