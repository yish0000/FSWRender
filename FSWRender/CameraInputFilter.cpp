/*
 * ------------------------------------------------------------------------
 *  Name:   CameraInputFilter.cpp
 *  Desc:   本文件实现了一个相机输入过滤器。
 *  Author: Yish
 *  Date:   2012/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#define _WIN32_WINNT 0x0400

#include "FSWTypes.h"
#include "CameraInputFilter.h"
#include "FSWCamera.h"
#include "Timer.h"
#include "WinApp.h"


const float CAMERA_MOVE_SPEED = 1.0f;
const float CAMERA_ROTATE_SPEED = FSW_PI*1.0f;

/** 构造函数
*/
CameraInputFilter::CameraInputFilter(InputManager* pManager, FSWCamera* pCamera)
	: InputFilter(pManager), m_pCamera(pCamera), m_bTurnCamera(false), m_bShowCursor(true)
{
	RegisterLogicKey(LKEY_MOVE_LEFT, 'A', COMBO_KEY_NONE, VK_LEFT, COMBO_KEY_NONE);
	RegisterLogicKey(LKEY_MOVE_RIGHT, 'D', COMBO_KEY_NONE, VK_RIGHT, COMBO_KEY_NONE);
	RegisterLogicKey(LKEY_MOVE_DOWN, 'S', COMBO_KEY_NONE, VK_DOWN, COMBO_KEY_NONE);
	RegisterLogicKey(LKEY_MOVE_UP, 'W', COMBO_KEY_NONE, VK_UP, COMBO_KEY_NONE);
}

/** 处理输入
*/
bool CameraInputFilter::TranslateInput()
{
	// 按键控制相机移动
	OnKeyboardMove();

	// 响应鼠标消息
	for( size_t i=0;i<m_pManager->GetMouseMessageCount();i++ )
	{
		const InputManager::WINMSG& msg = m_pManager->GetMouseMessage(i);

		int x = (int)(short)LOWORD(msg.lParam);
		int y = (int)(short)HIWORD(msg.lParam);
		switch( msg.uMsg )
		{
		case WM_LBUTTONDOWN:	OnLButtonDown(x, y, msg.dwCtrlKeys);	break;
		case WM_LBUTTONDBLCLK:	OnLButtonDblClk(x, y, msg.dwCtrlKeys);	break;
		case WM_LBUTTONUP:		OnLButtonUp(x, y, msg.dwCtrlKeys);		break;
		case WM_RBUTTONDOWN:	OnRButtonDown(x, y, msg.dwCtrlKeys);	break;
		case WM_RBUTTONDBLCLK:	OnRButtonDblClk(x, y, msg.dwCtrlKeys);	break;
		case WM_RBUTTONUP:		OnRButtonUp(x, y, msg.dwCtrlKeys);		break;
		case WM_MOUSEMOVE:		OnMouseMove(x, y, msg.dwCtrlKeys);		break;
		case WM_MOUSEWHEEL:
			{
				int iDelta = (int)(short)HIWORD(msg.wParam);
				DWORD dwFlags = LOWORD(msg.wParam);
				OnMouseWheel(x, y, iDelta, dwFlags);
				break;
			}
		}
	}

	return true;
}

/** 按键控制相机移动
*/
void CameraInputFilter::OnKeyboardMove()
{
	float fFast = 10.0f;
	float fElapsed = WinApp::GetInstance()->GetTimer()->GetElapsed();

	float fMove = CAMERA_MOVE_SPEED * fElapsed;
	if( m_pManager->IsShiftPressed() )
		fMove *= fFast;

	if( IsLogicKeyPressed(LKEY_MOVE_UP) )
		m_pCamera->MoveLocalZ(fMove);

	if( IsLogicKeyPressed(LKEY_MOVE_DOWN) )
		m_pCamera->MoveLocalZ(-fMove);

	if( IsLogicKeyPressed(LKEY_MOVE_LEFT) )
		m_pCamera->MoveLocalX(-fMove);

	if( IsLogicKeyPressed(LKEY_MOVE_RIGHT) )
		m_pCamera->MoveLocalX(fMove);
}

/** 响应鼠标左键按下
*/
void CameraInputFilter::OnLButtonDown( int x,int y,DWORD dwFlags )
{
}

/** 响应鼠标左键双击
*/
void CameraInputFilter::OnLButtonDblClk( int x,int y,DWORD dwFlags )
{
}

/** 响应鼠标左键抬起
*/
void CameraInputFilter::OnLButtonUp( int x,int y,DWORD dwFlags )
{
}

/** 响应鼠标右键按下
*/
void CameraInputFilter::OnRButtonDown( int x,int y,DWORD dwFlags )
{
	ShowCursor(false);
	m_pManager->CaptureMouse(true);
	m_ptBeginPoint.x = m_ptLastPoint.x = x;
	m_ptBeginPoint.y = m_ptLastPoint.y = y;
	m_bTurnCamera = true;
}

/** 响应鼠标右键双击
*/
void CameraInputFilter::OnRButtonDblClk( int x,int y,DWORD dwFlags )
{
	OnRButtonDown(x, y, dwFlags);
}

/** 响应鼠标右键抬起
*/
void CameraInputFilter::OnRButtonUp( int x,int y,DWORD dwFlags )
{
	ShowCursor(true);
	m_pManager->CaptureMouse(false);

	if( m_bTurnCamera )
	{
		HWND hWnd = WinApp::GetInstance()->GetMainWnd();
		::ClientToScreen(hWnd, &m_ptBeginPoint);
		::SetCursorPos(m_ptBeginPoint.x, m_ptBeginPoint.y);
		m_bTurnCamera = false;
	}
}

/** 响应鼠标移动
*/
void CameraInputFilter::OnMouseMove( int x,int y,DWORD dwFlags )
{
	float fElapsed = WinApp::GetInstance()->GetTimer()->GetElapsed();

	if( m_bTurnCamera )
	{
		float fX = (float)(x - m_ptLastPoint.x);
		float fY = (float)(y - m_ptLastPoint.y);
		if( fX > 0 ) m_pCamera->Yaw( CAMERA_ROTATE_SPEED*fElapsed );
		if( fX < 0 ) m_pCamera->Yaw( -CAMERA_ROTATE_SPEED*fElapsed );
		if( fY > 0 ) m_pCamera->Pitch( CAMERA_ROTATE_SPEED*fElapsed );
		if( fY < 0 ) m_pCamera->Pitch( -CAMERA_ROTATE_SPEED*fElapsed );

		RECT rcClient;
		HWND hWnd = WinApp::GetInstance()->GetMainWnd();
		::GetClientRect(hWnd, &rcClient);
		if( x < rcClient.left || x > rcClient.right ||
			y < rcClient.top || y > rcClient.bottom )
		{
			POINT pt = m_ptBeginPoint;
			::ClientToScreen(hWnd, &pt);
			::SetCursorPos(pt.x, pt.y);
			m_ptLastPoint = m_ptBeginPoint;
		}
		else
		{
			m_ptLastPoint.x = x;
			m_ptLastPoint.y = y;
		}
	}
}

/** 响应鼠标滚轮
*/
void CameraInputFilter::OnMouseWheel( int x,int y,int iDelta,DWORD dwFlags )
{
}

/** 显示/隐藏鼠标
*/
void CameraInputFilter::ShowCursor( bool bShow )
{
	if( bShow && !m_bShowCursor )
	{
		m_bShowCursor = true;
		::ShowCursor(TRUE);
	}
	else if( !bShow && m_bShowCursor )
	{
		m_bShowCursor = false;
		::ShowCursor(FALSE);
	}
}