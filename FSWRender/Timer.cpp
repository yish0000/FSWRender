/*
 * -----------------------------------------------------------------------
 *	Name:   Timer.cpp
 *  Desc:   ���ļ�����ʵ�������ʱ�ӿ�����FPS����ȡ�
 *  Author: Yish
 *  Date:   8/8/2010
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#include "Timer.h"


/** ���캯��
*/
Timer::Timer(void) : m_nCurTime(0),m_fElapsed(0.0f),m_fFPS(0.0f)
{
    __int64 freq;
    if( QueryPerformanceFrequency((LARGE_INTEGER*)&freq) )
    {
        m_fTimeFreq = 1.0f / freq;
        QueryPerformanceCounter( (LARGE_INTEGER*)&m_nLastTime );
        m_bCPUPerf = true;
    }
    else
    {
        m_fTimeFreq = 0.001f;
        m_nLastTime = (__int64)GetTickCount();
        m_bCPUPerf = false;
    }
}

/** ��������
*/
Timer::~Timer(void)
{
}

/** ��ÿһ֡���������ʱ�Ӷ���
@remarks
    ������ε��ø�����FPS,�򷵻�true
*/
bool Timer::Update(void)
{
    static long s_nFrames = 0;
    static float s_fAccumulate = 0.0f;

    if( m_bCPUPerf )
        QueryPerformanceCounter( (LARGE_INTEGER*)&m_nCurTime );
    else
        m_nCurTime = (__int64)GetTickCount();

    m_fElapsed = (m_nCurTime - m_nLastTime) * m_fTimeFreq;
    m_nLastTime = m_nCurTime;

	// ÿ��һ�����һ�����浱ǰ��FPS
    s_nFrames++;
    s_fAccumulate += m_fElapsed;

    if( s_fAccumulate > 1.0f )
    {
        m_fFPS = s_nFrames / s_fAccumulate;

        s_fAccumulate = 0.0f;
        s_nFrames = 0;
        return true;
    }

    return false;
}