/*
 * ------------------------------------------------------------------------
 *  Name:   InputFilter.h
 *  Desc:   本文件定义了一个输入过滤器，负责程序的输入行为。
 *  Author: Yish
 *  Date:   2012/11/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __INPUTFILTER_H__
#define __INPUTFILTER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Common.h"

///////////////////////////////////////////////////////////////////////////


#define KEY_CTRL_PRESSED	0x0001
#define KEY_ALT_PRESSED		0x0002
#define KEY_SHIFT_PRESSED	0x0004
#define KEY_CKEY_PRESSED	0x0007
#define KEY_FIRSTPRESS		0x0100

#define NUM_VIRTUALKEY		256

///////////////////////////////////////////////////////////////////////////


class InputManager;

/** 输入过滤器基类
*/
class InputFilter
{
public:

	enum
	{
		COMBO_KEY_SCA,
		COMBO_KEY_CA,
		COMBO_KEY_SA,
		COMBO_KEY_SC,
		COMBO_KEY_S,
		COMBO_KEY_C,
		COMBO_KEY_A,
		COMBO_KEY_NONE,
		COMBO_KEY_NUM,
	};

	struct VK_GROUP
	{
		DWORD dwVK1;
		DWORD dwVK2;
	};

	typedef std::map<int, VK_GROUP> LK2VKMap;

public:
	InputFilter( InputManager* pManager );
	virtual ~InputFilter();

	// 处理输入
	virtual bool TranslateInput() = 0;

	// 检测逻辑按键是否被按下
	bool IsLogicKeyPressed(int iLogicKey) const;

	// 找指定的逻辑按键
	int GetLogicKey(int vk, DWORD dwCtrlKeys) const;
	// 查找逻辑按键的虚拟按键
	bool GetVirtualKey(int lk, int& vk1, DWORD& dwCtrlKeys1, int& vk2, DWORD& dwCtrlKeys2) const;
	// 重新设置逻辑按键
	bool RemapLogicKey(int lk, int vk1, DWORD dwCtrlKeys1, int vk2, DWORD dwCtrlKeys2);

protected:
	InputManager* m_pManager;

	int m_VK2LKMap[NUM_VIRTUALKEY][COMBO_KEY_NUM];
	LK2VKMap m_LK2VKMap;

	// 各类型组合键
	static DWORD s_dwComboKey[COMBO_KEY_NUM];

protected:

	// 将虚拟按键压缩成一个双字
	DWORD CompressVK(int vk, DWORD dwCtrlKeys) const;
	// 解压虚拟按键信息
	void DecompressVK(DWORD dwVK, int& vk, DWORD& dwCtrlKeys) const;

	// 注册逻辑按键
	void RegisterLogicKey(int lk, int vk1, int iCombo1, int vk2, int iCombo2);
	// 清除逻辑按键
	void ClearLogicKey(int lk);
	// 清除所有逻辑按键
	void ClearAllLogicKeys();

	// 获取组合键的类型
	int GetComboKeyIndex(DWORD dwCtrlKeys) const;
};

///////////////////////////////////////////////////////////////////////////

/** 输入控制器
*/
class InputManager
{
public:

	struct WINMSG
	{
		UINT uMsg;
		WPARAM wParam;
		LPARAM lParam;
		DWORD dwCtrlKeys;
	};

	typedef std::vector<WINMSG> MessageList;
	typedef std::vector<InputFilter*> InputFilterChain;

public:
	InputManager();
	virtual ~InputManager();

	// 添加删除过滤器
	bool RegisterFilter( InputFilter* pFilter );
	void RemoveFilter( InputFilter* pFilter );
	void RemoveAllFilters();

	// 更新过滤器
	void Tick();

	// 处理WINDOWS消息
	void OnKeyMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 检测某个按键是否被按下
	bool IsKeyPressed( int vk ) const;
	bool IsCtrlPressed() const;
	bool IsAltPressed() const;
	bool IsShiftPressed() const;

	// 捕获鼠标
	void CaptureMouse( bool bCapture );
	bool IsMouseCaptured() const;

	// 鼠标位置
	void GetMousePos( int* px, int* py ) const;
	void SetMousePos( int x, int y );

	// 获取键盘消息
	size_t GetKeyMessageCount() const { return m_KeyMsg.size(); }
	const WINMSG& GetKeyMessage(size_t idx) const
	{
		ASSERT(idx < m_KeyMsg.size());
		return m_KeyMsg[idx];
	}

	// 获取鼠标消息
	size_t GetMouseMessageCount() const { return m_MouseMsg.size(); }
	const WINMSG& GetMouseMessage(size_t idx) const
	{
		ASSERT(idx < m_MouseMsg.size());
		return m_MouseMsg[idx];
	}

	DWORD GetCtrlKeyStates() const { return m_dwCtrlFlags; }

protected:
	MessageList m_MouseMsg;			// 鼠标消息
	MessageList m_KeyMsg;			// 键盘消息
	bool m_bKeyPressed[NUM_VIRTUALKEY];	// 按键状态
	DWORD m_dwCtrlFlags;			// 控制键的状态
	int m_iMouseCapture;			// 捕获鼠标的次数

	InputFilterChain m_Filters;		// 输入过滤器链

private:

	// 检查虚拟按键状态
	void CheckKeyPressStates();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __INPUTFILTER_H__