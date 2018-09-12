/*
 * ------------------------------------------------------------------------
 *  Name:   CameraInputFilter.h
 *  Desc:   ���ļ�������һ����������������������
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

/** �������
*/
class CameraInputFilter : public InputFilter
{
	enum
	{
		LKEY_NONE,			// ���ã���ֵ�豣��
		LKEY_MOVE_LEFT,		// ����
		LKEY_MOVE_RIGHT,	// ����
		LKEY_MOVE_UP,		// ����
		LKEY_MOVE_DOWN,		// ����
	};

public:
	CameraInputFilter(InputManager* pManager, FSWCamera* pCamera);

	// ��������
	bool TranslateInput();

	// ��ȡ���ָ��
	FSWCamera* GetCamera() const { return m_pCamera; }
	// �������ָ��
	void SetCamera(FSWCamera* pCamera) { m_pCamera = pCamera; }

protected:
	FSWCamera* m_pCamera;
	bool m_bTurnCamera;
	POINT m_ptBeginPoint;
	POINT m_ptLastPoint;
	bool m_bShowCursor;

	// ��Ӧ����������
	void OnLButtonDown( int x,int y,DWORD dwFlags );
	// ��Ӧ������̧��
	void OnLButtonUp( int x,int y,DWORD dwFlags );
	// ��Ӧ���˫��
	void OnLButtonDblClk( int x,int y,DWORD dwFlags );
	// ��Ӧ����Ҽ�����
	void OnRButtonDown( int x,int y,DWORD dwFlags );
	// ��Ӧ����Ҽ�̧��
	void OnRButtonUp( int x,int y,DWORD dwFlags );
	// ��Ӧ����Ҽ�˫��
	void OnRButtonDblClk( int x,int y,DWORD dwFlags );
	// ��Ӧ����ƶ�
	void OnMouseMove( int x,int y,DWORD dwFlags );
	// ��Ӧ������
	void OnMouseWheel( int x,int y,int iDelta,DWORD dwFlags );

	// ��Ӧ��ס���̵��ƶ�
	void OnKeyboardMove();

	// ��ʾ/�������
	void ShowCursor( bool bShow );
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __CAMERAINPUTFILTER_H__