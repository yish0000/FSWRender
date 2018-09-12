/*
 * -----------------------------------------------------------------------
 *  Name:   WinApp.h
 *  Desc:   本文件定义了一个窗口应用程序对象。
 *  Author: Yish
 *  Date:   2011/7/20
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __WINAPP_H__
#define __WINAPP_H__


//////////////////////////////////////////////////////////////////////////
// HEADERS OF THIS FILE
#include "Common.h"
#include "FSWCamera.h"
#include "Timer.h"

//////////////////////////////////////////////////////////////////////////


class FSWDevice;
class FSWTexture;
class FSWScene;
class InputManager;
class InputFilter;
class FrameListener;

/** 应用程序对象
*/
class WinApp
{
	typedef std::vector<FrameListener*> FrameListenerList;

protected:
    HINSTANCE m_hInst;			// 应用程序的实例句柄
    HWND m_hWnd;				// 应用程序的窗口句柄
	bool m_bActive;				// 当前窗口是否活动
	bool m_bFullscreen;			// 当前是否为全屏模式
	DWORD m_dwStyle;			// 窗口风格

    FSWDevice* m_pDevice;		// 图形设备的指针
    Timer m_Timer;				// 时钟
	InputManager* m_pInput;		// 输入管理器
	InputFilter* m_pSysFilter;	// 系统输入过滤器
	InputFilter* m_pCamFilter;	// 相机输入过滤器

	FSWScene* m_pScene;			// 场景数据
	FrameListenerList m_Listeners; // 帧监听器列表

public:
    WinApp();
    ~WinApp();

    bool Init( HINSTANCE hInst );
    int Run(void);
    int ExitInstance(void);

	// 切换全屏模式
	void ToggleFullscreen();

	// 停止更新帧
	void SetActive(bool bActive) { m_bActive = bActive; }

	// 获取窗口句柄
	HWND GetMainWnd() const { return m_hWnd; }

	// 获取渲染设备指针
	FSWDevice* GetDevice() { return m_pDevice; }

	// 获取时钟对象
	Timer* GetTimer() { return &m_Timer; }

	// 是否是全屏模式
	bool IsFullscreen() const { return m_bFullscreen; }

	// 获取输入管理
	InputManager* GetInputManager() { return m_pInput; }

    // 窗口的回调函数
    static LRESULT CALLBACK MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    // 静态实例指针
    static WinApp* GetInstance();

private:
    // 初始化图形环境
    bool Init3DEngine();
	// 初始化场景
	bool OneTimeSceneInit();

    // 绘制每一帧
    void OnFrame();
	// 更新逻辑
	void OnTick();
	// 渲染场景
	void OnRender();

	static WinApp* s_pInst;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __WINAPP_H__