/*
 * -----------------------------------------------------------------------
 *  Name:   WinApp.h
 *  Desc:   ���ļ�������һ������Ӧ�ó������
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

/** Ӧ�ó������
*/
class WinApp
{
	typedef std::vector<FrameListener*> FrameListenerList;

protected:
    HINSTANCE m_hInst;			// Ӧ�ó����ʵ�����
    HWND m_hWnd;				// Ӧ�ó���Ĵ��ھ��
	bool m_bActive;				// ��ǰ�����Ƿ�
	bool m_bFullscreen;			// ��ǰ�Ƿ�Ϊȫ��ģʽ
	DWORD m_dwStyle;			// ���ڷ��

    FSWDevice* m_pDevice;		// ͼ���豸��ָ��
    Timer m_Timer;				// ʱ��
	InputManager* m_pInput;		// ���������
	InputFilter* m_pSysFilter;	// ϵͳ���������
	InputFilter* m_pCamFilter;	// ������������

	FSWScene* m_pScene;			// ��������
	FrameListenerList m_Listeners; // ֡�������б�

public:
    WinApp();
    ~WinApp();

    bool Init( HINSTANCE hInst );
    int Run(void);
    int ExitInstance(void);

	// �л�ȫ��ģʽ
	void ToggleFullscreen();

	// ֹͣ����֡
	void SetActive(bool bActive) { m_bActive = bActive; }

	// ��ȡ���ھ��
	HWND GetMainWnd() const { return m_hWnd; }

	// ��ȡ��Ⱦ�豸ָ��
	FSWDevice* GetDevice() { return m_pDevice; }

	// ��ȡʱ�Ӷ���
	Timer* GetTimer() { return &m_Timer; }

	// �Ƿ���ȫ��ģʽ
	bool IsFullscreen() const { return m_bFullscreen; }

	// ��ȡ�������
	InputManager* GetInputManager() { return m_pInput; }

    // ���ڵĻص�����
    static LRESULT CALLBACK MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    // ��̬ʵ��ָ��
    static WinApp* GetInstance();

private:
    // ��ʼ��ͼ�λ���
    bool Init3DEngine();
	// ��ʼ������
	bool OneTimeSceneInit();

    // ����ÿһ֡
    void OnFrame();
	// �����߼�
	void OnTick();
	// ��Ⱦ����
	void OnRender();

	static WinApp* s_pInst;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __WINAPP_H__