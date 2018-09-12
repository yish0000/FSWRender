/*
 * ------------------------------------------------------------------------
 *  Name:   InputFilter.cpp
 *  Desc:   本文件实现了程序的输入功能。
 *  Author: Yish
 *  Date:   2012/11/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "InputFilter.h"
#include "WinApp.h"


///////////////////////////////////////////////////////////////////////////
//  
//  Implement class InputFilter
//  
///////////////////////////////////////////////////////////////////////////

// 各类型组合键
DWORD InputFilter::s_dwComboKey[InputFilter::COMBO_KEY_NUM] =
{
	KEY_CTRL_PRESSED | KEY_ALT_PRESSED | KEY_SHIFT_PRESSED,
	KEY_CTRL_PRESSED | KEY_ALT_PRESSED,
	KEY_SHIFT_PRESSED | KEY_ALT_PRESSED,
	KEY_SHIFT_PRESSED | KEY_CTRL_PRESSED,
	KEY_SHIFT_PRESSED,
	KEY_CTRL_PRESSED,
	KEY_ALT_PRESSED,
	0,
};

/** 构造函数
*/
InputFilter::InputFilter( InputManager* pManager ) : m_pManager(pManager)
{
	memset(m_VK2LKMap, 0, sizeof(m_VK2LKMap));
}

/** 析构函数
*/
InputFilter::~InputFilter()
{
}

/** 检测逻辑按键是否按下
*/
bool InputFilter::IsLogicKeyPressed(int iLogicKey) const
{
	int vk;
	DWORD dwCtrlKeys;

	LK2VKMap::const_iterator it = m_LK2VKMap.find(iLogicKey);
	if( it != m_LK2VKMap.end() )
	{
		DWORD dwCurCtrlKeys = m_pManager->GetCtrlKeyStates();
		dwCurCtrlKeys &= KEY_CKEY_PRESSED;

		const VK_GROUP& vk_gp = it->second;

		DecompressVK(vk_gp.dwVK1, vk, dwCtrlKeys);
		if( vk && (m_pManager->IsKeyPressed(vk) && (dwCtrlKeys & dwCurCtrlKeys) == dwCtrlKeys) )
			return true;

		DecompressVK(vk_gp.dwVK2, vk, dwCtrlKeys);
		if( vk && (m_pManager->IsKeyPressed(vk) && (dwCtrlKeys & dwCurCtrlKeys) == dwCtrlKeys) )
			return true;
	}

	return false;
}

/** 查找指定的逻辑按键
*/
int InputFilter::GetLogicKey(int vk, DWORD dwCtrlKeys) const
{
	if( vk <= 0 || vk >= NUM_VIRTUALKEY )
		return 0;

	int idx = GetComboKeyIndex(dwCtrlKeys);
	return m_VK2LKMap[vk][idx];
}

/** 查找指定的虚拟按键
*/
bool InputFilter::GetVirtualKey(int lk, int& vk1, DWORD& dwCtrlKeys1, int& vk2, DWORD& dwCtrlKeys2) const
{
	vk1 = vk2 = 0;
	dwCtrlKeys1 = dwCtrlKeys2 = 0;

	LK2VKMap::const_iterator it = m_LK2VKMap.find(lk);
	if( it != m_LK2VKMap.end() )
	{
		const VK_GROUP& vk_gp = it->second;

		DecompressVK(vk_gp.dwVK1, vk1, dwCtrlKeys1);
		DecompressVK(vk_gp.dwVK2, vk2, dwCtrlKeys2);
		return true;
	}

	return false;
}

/** 重新设置逻辑按键
*/
bool InputFilter::RemapLogicKey(int lk, int vk1, DWORD dwCtrlKeys1, int vk2, DWORD dwCtrlKeys2)
{
	LK2VKMap::iterator it = m_LK2VKMap.find(lk);
	if( it == m_LK2VKMap.end() ) return false;

	// 清理旧的逻辑按键
	ClearLogicKey(lk);

	// 重新注册逻辑按键
	int iComboKey1 = GetComboKeyIndex(dwCtrlKeys1);
	int iComboKey2 = GetComboKeyIndex(dwCtrlKeys2);
	RegisterLogicKey(lk, vk1, iComboKey1, vk2, iComboKey2);

	return true;
}

/** 将虚拟按键压缩成一个双字
*/
DWORD InputFilter::CompressVK(int vk, DWORD dwCtrlKeys) const
{
	return (vk << 16) | (dwCtrlKeys & KEY_CKEY_PRESSED);
}

/** 解压虚拟按键数据
*/
void InputFilter::DecompressVK(DWORD dwVK, int& vk, DWORD& dwCtrlKeys) const
{
	dwCtrlKeys = dwVK & KEY_CKEY_PRESSED;
	vk = (int)(dwVK >> 16);
}

/** 注册逻辑按键
*/
void InputFilter::RegisterLogicKey(int lk, int vk1, int iCombo1, int vk2, int iCombo2)
{
	DWORD dwCtrlKeys1 = s_dwComboKey[iCombo1];
	DWORD dwCtrlKeys2 = s_dwComboKey[iCombo2];

	if( vk1 ) m_VK2LKMap[vk1][iCombo1] = lk;
	if( vk2 ) m_VK2LKMap[vk2][iCombo2] = lk;

	VK_GROUP vk_gp;
	vk_gp.dwVK1 = vk1 ? CompressVK(vk1, dwCtrlKeys1) : 0;
	vk_gp.dwVK2 = vk2 ? CompressVK(vk2, dwCtrlKeys2) : 0;
	m_LK2VKMap[lk] = vk_gp;
}

/** 清除指定的逻辑按键
*/
void InputFilter::ClearLogicKey(int lk)
{
	int vk, iCombo;
	DWORD dwCtrlKeys;
	VK_GROUP& vk_gp = m_LK2VKMap[lk];

	// 清理第一组VK
	DecompressVK(vk_gp.dwVK1, vk, dwCtrlKeys);
	iCombo = GetComboKeyIndex(dwCtrlKeys);
	if( vk && m_VK2LKMap[vk][iCombo] == lk )
		m_VK2LKMap[vk][iCombo] = 0;

	// 清理第二组VK
	DecompressVK(vk_gp.dwVK2, vk, dwCtrlKeys);
	iCombo = GetComboKeyIndex(dwCtrlKeys);
	if( vk && m_VK2LKMap[vk][iCombo] == lk )
		m_VK2LKMap[vk][iCombo] = 0;

	vk_gp.dwVK1 = 0;
	vk_gp.dwVK2 = 0;
}

/** 清理所有的逻辑按键
*/
void InputFilter::ClearAllLogicKeys()
{
	m_LK2VKMap.clear();
	memset(m_VK2LKMap, 0, sizeof(m_VK2LKMap));
}

/** 获取组合键的类型
*/
int InputFilter::GetComboKeyIndex(DWORD dwCtrlKeys) const
{
	int i;
	dwCtrlKeys &= KEY_CKEY_PRESSED;

	for( i=0;i<COMBO_KEY_NUM;i++ )
	{
		if( s_dwComboKey[i] == dwCtrlKeys )
			return i;
	}

	ASSERT(i < COMBO_KEY_NUM);
	return i;
}


///////////////////////////////////////////////////////////////////////////
//  
//  Implement class InputManager
//  
///////////////////////////////////////////////////////////////////////////

/** 构造函数
*/
InputManager::InputManager()
{
	m_iMouseCapture = 0;
	m_dwCtrlFlags = 0;
	memset(m_bKeyPressed, 0, sizeof(m_bKeyPressed));
}

/** 析构函数
*/
InputManager::~InputManager()
{
}

/** 注册一个输入过滤器
*/
bool InputManager::RegisterFilter( InputFilter* pFilter )
{
	InputFilterChain::iterator it;
	if( (it = std::find(m_Filters.begin(), m_Filters.end(), pFilter)) != m_Filters.end() )
		return false;

	m_Filters.push_back(pFilter);
	return true;
}

/** 删除指定的过滤器
*/
void InputManager::RemoveFilter( InputFilter* pFilter )
{
	InputFilterChain::iterator it;
	if( (it = std::find(m_Filters.begin(), m_Filters.end(), pFilter)) != m_Filters.end() )
		m_Filters.erase(it);
}

/** 移除所有的过滤器
*/
void InputManager::RemoveAllFilters()
{
	m_Filters.clear();
}

/** 更新输入管理器
*/
void InputManager::Tick()
{
	CheckKeyPressStates();

	InputFilterChain::iterator it;
	for( it=m_Filters.begin();it!=m_Filters.end();++it )
		(*it)->TranslateInput();

	m_KeyMsg.clear();
	m_MouseMsg.clear();
}

/** 更新虚拟按键状态
*/
void InputManager::CheckKeyPressStates()
{
	if( GetAsyncKeyState(VK_CONTROL) & 0x8000 )
		m_dwCtrlFlags |= KEY_CTRL_PRESSED;
	else
		m_dwCtrlFlags &= ~KEY_CTRL_PRESSED;

	if( GetAsyncKeyState(VK_MENU) & 0x8000 )
		m_dwCtrlFlags |= KEY_ALT_PRESSED;
	else
		m_dwCtrlFlags &= ~KEY_ALT_PRESSED;

	if( GetAsyncKeyState(VK_SHIFT) & 0x8000 )
		m_dwCtrlFlags |= KEY_SHIFT_PRESSED;
	else
		m_dwCtrlFlags &= ~KEY_SHIFT_PRESSED;

	for( int i=0;i<NUM_VIRTUALKEY;i++ )
	{
		if( !(GetAsyncKeyState(i) & 0x8000) )
			m_bKeyPressed[i] = false;
	}
}

/** 响应键盘消息的到来
*/
void InputManager::OnKeyMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool bGenMsg = false;
	bool bFirstPress = false;

	switch( uMsg )
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if( wParam == VK_CONTROL )
			m_dwCtrlFlags |= KEY_CTRL_PRESSED;
		else if( wParam == VK_MENU )
			m_dwCtrlFlags |= KEY_ALT_PRESSED;
		else if( wParam == VK_SHIFT )
			m_dwCtrlFlags |= KEY_SHIFT_PRESSED;
		else
			bGenMsg = true;

		if( !m_bKeyPressed[wParam] )
			bFirstPress = true;

		m_bKeyPressed[wParam] = true;
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		if( wParam == VK_CONTROL )
			m_dwCtrlFlags &= ~KEY_CTRL_PRESSED;
		else if( wParam == VK_MENU )
			m_dwCtrlFlags &= ~KEY_ALT_PRESSED;
		else if( wParam == VK_SHIFT )
			m_dwCtrlFlags &= ~KEY_SHIFT_PRESSED;
		else
			bGenMsg = true;

		m_bKeyPressed[wParam] = false;
		break;
	}

	if( bGenMsg )
	{
		WINMSG msg;
		msg.uMsg = uMsg;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.dwCtrlKeys = m_dwCtrlFlags;

		if( bFirstPress )
			msg.dwCtrlKeys |= KEY_FIRSTPRESS;

		m_KeyMsg.push_back(msg);
	}
}

/** 响应鼠标消息的到来
*/
void InputManager::OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WINMSG msg;
	msg.uMsg = uMsg;
	msg.wParam = wParam;
	msg.lParam = lParam;
	msg.dwCtrlKeys = 0;

	if( wParam & MK_CONTROL )
		msg.dwCtrlKeys |= KEY_CTRL_PRESSED;

	if( wParam & MK_SHIFT )
		msg.dwCtrlKeys |= KEY_SHIFT_PRESSED;

	if( m_dwCtrlFlags & KEY_ALT_PRESSED )
		msg.dwCtrlKeys |= KEY_ALT_PRESSED;

	m_MouseMsg.push_back(msg);
}

// 指定虚拟按键是否被按下
bool InputManager::IsKeyPressed( int vk ) const
{
	ASSERT(vk >= 0 && vk < NUM_VIRTUALKEY);
	return m_bKeyPressed[vk];
}

// 检测Ctrl键是否被按下
bool InputManager::IsCtrlPressed() const
{
	return (m_dwCtrlFlags & KEY_CTRL_PRESSED) ? true : false;
}

// 检测Alt键是否被按下
bool InputManager::IsAltPressed() const
{
	return (m_dwCtrlFlags & KEY_ALT_PRESSED) ? true : false;
}

// 检测Shift键是否被按下
bool InputManager::IsShiftPressed() const
{
	return (m_dwCtrlFlags & KEY_SHIFT_PRESSED) ? true : false;
}

// 捕获鼠标
void InputManager::CaptureMouse( bool bCapture )
{
	HWND hWnd = WinApp::GetInstance()->GetMainWnd();

	if( bCapture )
	{
		if( GetCapture() != hWnd )
		{
			SetCapture(hWnd);
			m_iMouseCapture = 1;
		}
		else
		{
			m_iMouseCapture++;
		}
	}
	else
	{
		if( GetCapture() == hWnd )
		{
			if( !(--m_iMouseCapture) )
				ReleaseCapture();
		}
	}
}

// 检测鼠标是否被该窗口捕获
bool InputManager::IsMouseCaptured() const
{
	return GetCapture() == WinApp::GetInstance()->GetMainWnd();
}

// 获取鼠标的位置
void InputManager::GetMousePos( int* px, int* py ) const
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(WinApp::GetInstance()->GetMainWnd(), &pt);
	*px = pt.x;
	*py = pt.y;
}

// 设置鼠标的位置
void InputManager::SetMousePos( int x, int y )
{
	POINT pt = {x, y};
	ClientToScreen(WinApp::GetInstance()->GetMainWnd(), &pt);
	SetCursorPos(pt.x, pt.y);
}