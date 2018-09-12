/*
 * -----------------------------------------------------------------------
 *  Name:   Timer.h
 *  Desc:   ���ļ�Ϊ���涨����һ��ʱ���ࡣ
 *  Author: Yish
 *  Date:   2011/1/13
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __TIMER_H__
#define __TIMER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Common.h"

//////////////////////////////////////////////////////////////////////////


/** ʱ�ӿ�����
*/
class Timer
{
protected:
    __int64 m_nCurTime;     // ��ǰ��ʱ�Ӽ���
    __int64 m_nLastTime;    // ��һ֡��ʱ�Ӽ���
    float m_fTimeFreq;      // 1 / ʱ��Ƶ��
    float m_fElapsed;       // ��֡�����ĵ�ʱ��(��λ: ��)
    float m_fFPS;           // ��ǰ�����FPS
    bool m_bCPUPerf;        // �Ƿ�����CPU�߾��ȼ�ʱ��

public:
    Timer(void);
    ~Timer(void);

    /** ���������ʱ��
    @remarks
        �˺���Ӧ����Ϸѭ����ÿһ֡������
    */
    bool Update(void);

    // ��ȡ��֡�����ĵ�ʱ��
    float GetElapsed(void) const { return m_fElapsed; }

    // ��ȡ��ǰ��FPS
    float GetFPS(void) const { return m_fFPS; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __TIMER_H__