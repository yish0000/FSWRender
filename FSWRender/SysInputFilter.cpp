/*
 * ------------------------------------------------------------------------
 *  Name:   SysInputFilter.cpp
 *  Desc:   本文件实现了一个系统输入过滤器。
 *  Author: Yish
 *  Date:   2012/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "SysInputFilter.h"
#include "WinApp.h"
#include "FSWDevice.h"
#include <time.h>


/** 构造函数
*/
SysInputFilter::SysInputFilter( InputManager* pManager ) : InputFilter(pManager)
{
	RegisterLogicKey(LKEY_PRINTSCREEN, 'P', COMBO_KEY_NONE, 0, COMBO_KEY_NONE);
	RegisterLogicKey(LKEY_FULLSCREEN, VK_RETURN, COMBO_KEY_A, 0, COMBO_KEY_NONE);
	RegisterLogicKey(LKEY_WIREFRAME, VK_F1, COMBO_KEY_NONE, 0, COMBO_KEY_NONE);
}

/** 处理输入行为
*/
bool SysInputFilter::TranslateInput()
{
	for( size_t i=0;i<m_pManager->GetKeyMessageCount();i++ )
	{
		const InputManager::WINMSG& msg = m_pManager->GetKeyMessage(i);
		if( msg.uMsg == WM_KEYDOWN || msg.uMsg == WM_SYSKEYDOWN )
			OnKeyDown(msg.uMsg, msg.wParam, msg.dwCtrlKeys);
	}

	return true;
}

/** 响应键盘按下
*/
void SysInputFilter::OnKeyDown(UINT uMsg, WPARAM wParam, DWORD dwCtrlKeys)
{
	WinApp* app = WinApp::GetInstance();
	FSWDevice* pDevice = app->GetDevice();

	int iLogicKey = GetLogicKey((int)wParam, dwCtrlKeys);

	// 控制角色
    switch( iLogicKey )
    {
    case LKEY_PRINTSCREEN:
        PrintScreen();
        break;
    case LKEY_FULLSCREEN:
        app->ToggleFullscreen();
        break;
    case LKEY_WIREFRAME:
		if( pDevice->GetFillMode() == FFM_SOLID )
			pDevice->SetFillMode(FFM_WIREFRAME);
		else
			pDevice->SetFillMode(FFM_SOLID);
        break;
	}
}

/** 截取屏幕
*/
void SysInputFilter::PrintScreen()
{
	TCHAR szFilename[MAX_PATH];
	time_t t = time(NULL);
	tm* now = localtime(&t);

	FSWDevice* pDevice = WinApp::GetInstance()->GetDevice();
	if( !pDevice ) return;

	// 创建截图目录
	CreateDirectory("Screenshot", NULL);

	// 生成要保存的文件名
	_snprintf(szFilename, MAX_PATH, "%s\\%d_%d_%d__%d_%d_%d.bmp", "Screenshot",
		now->tm_year+1900, now->tm_mon+1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	pDevice->PrintScreen(szFilename);
}