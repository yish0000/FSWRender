/*
 * ------------------------------------------------------------------------
 *  Name:   InputFilter.h
 *  Desc:   ���ļ�������һ���������������������������Ϊ��
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

/** �������������
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

	// ��������
	virtual bool TranslateInput() = 0;

	// ����߼������Ƿ񱻰���
	bool IsLogicKeyPressed(int iLogicKey) const;

	// ��ָ�����߼�����
	int GetLogicKey(int vk, DWORD dwCtrlKeys) const;
	// �����߼����������ⰴ��
	bool GetVirtualKey(int lk, int& vk1, DWORD& dwCtrlKeys1, int& vk2, DWORD& dwCtrlKeys2) const;
	// ���������߼�����
	bool RemapLogicKey(int lk, int vk1, DWORD dwCtrlKeys1, int vk2, DWORD dwCtrlKeys2);

protected:
	InputManager* m_pManager;

	int m_VK2LKMap[NUM_VIRTUALKEY][COMBO_KEY_NUM];
	LK2VKMap m_LK2VKMap;

	// ��������ϼ�
	static DWORD s_dwComboKey[COMBO_KEY_NUM];

protected:

	// �����ⰴ��ѹ����һ��˫��
	DWORD CompressVK(int vk, DWORD dwCtrlKeys) const;
	// ��ѹ���ⰴ����Ϣ
	void DecompressVK(DWORD dwVK, int& vk, DWORD& dwCtrlKeys) const;

	// ע���߼�����
	void RegisterLogicKey(int lk, int vk1, int iCombo1, int vk2, int iCombo2);
	// ����߼�����
	void ClearLogicKey(int lk);
	// ��������߼�����
	void ClearAllLogicKeys();

	// ��ȡ��ϼ�������
	int GetComboKeyIndex(DWORD dwCtrlKeys) const;
};

///////////////////////////////////////////////////////////////////////////

/** ���������
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

	// ���ɾ��������
	bool RegisterFilter( InputFilter* pFilter );
	void RemoveFilter( InputFilter* pFilter );
	void RemoveAllFilters();

	// ���¹�����
	void Tick();

	// ����WINDOWS��Ϣ
	void OnKeyMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ���ĳ�������Ƿ񱻰���
	bool IsKeyPressed( int vk ) const;
	bool IsCtrlPressed() const;
	bool IsAltPressed() const;
	bool IsShiftPressed() const;

	// �������
	void CaptureMouse( bool bCapture );
	bool IsMouseCaptured() const;

	// ���λ��
	void GetMousePos( int* px, int* py ) const;
	void SetMousePos( int x, int y );

	// ��ȡ������Ϣ
	size_t GetKeyMessageCount() const { return m_KeyMsg.size(); }
	const WINMSG& GetKeyMessage(size_t idx) const
	{
		ASSERT(idx < m_KeyMsg.size());
		return m_KeyMsg[idx];
	}

	// ��ȡ�����Ϣ
	size_t GetMouseMessageCount() const { return m_MouseMsg.size(); }
	const WINMSG& GetMouseMessage(size_t idx) const
	{
		ASSERT(idx < m_MouseMsg.size());
		return m_MouseMsg[idx];
	}

	DWORD GetCtrlKeyStates() const { return m_dwCtrlFlags; }

protected:
	MessageList m_MouseMsg;			// �����Ϣ
	MessageList m_KeyMsg;			// ������Ϣ
	bool m_bKeyPressed[NUM_VIRTUALKEY];	// ����״̬
	DWORD m_dwCtrlFlags;			// ���Ƽ���״̬
	int m_iMouseCapture;			// �������Ĵ���

	InputFilterChain m_Filters;		// �����������

private:

	// ������ⰴ��״̬
	void CheckKeyPressStates();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __INPUTFILTER_H__