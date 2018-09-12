/*
 * ------------------------------------------------------------------------
 *  Name:   SysInputFilter.h
 *  Desc:   ���ļ�������һ��ϵͳ�������������
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


/** ϵͳ���������
*/
class SysInputFilter : public InputFilter
{
	enum
	{
		LKEY_NONE,			// ��ֵ���ã��豣��
		LKEY_PRINTSCREEN,	// ��ȡ��ǰ��Ļ
		LKEY_FULLSCREEN,	// �л�ȫ��ģʽ
		LKEY_WIREFRAME,		// �л��߿�ģʽ
		LKEY_SHOWFPS,		// �л���ʾFPS

		LKEY_SELECTMODE1,	// ѡ������ģʽ
		LKEY_SELECTMODE2,
		LKEY_SELECTMODE3,
		LKEY_SELECTMODE4,
		LKEY_SELECTMODE5,
		LKEY_SELECTMODE6,

		LKEY_QUALITY_LOW,	// ������Դ�Ĵ�С
		LKEY_QUALITY_MIDDLE,
		LKEY_QUALITY_HIGH,
	};

public:
	SysInputFilter( InputManager* pManager );

	// ��������
	bool TranslateInput();

private:

	// ��Ӧ���̰���
	void OnKeyDown(UINT uMsg, WPARAM wParam, DWORD dwCtrlKeys);

	// ��ȡ��Ļ
	void PrintScreen();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __SYSINPUTFILTER_H__