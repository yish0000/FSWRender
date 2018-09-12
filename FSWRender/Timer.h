/*
 * -----------------------------------------------------------------------
 *  Name:   Timer.h
 *  Desc:   本文件为引擎定义了一个时钟类。
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


/** 时钟控制器
*/
class Timer
{
protected:
    __int64 m_nCurTime;     // 当前的时钟计数
    __int64 m_nLastTime;    // 上一帧的时钟计数
    float m_fTimeFreq;      // 1 / 时钟频率
    float m_fElapsed;       // 本帧所消耗的时间(单位: 秒)
    float m_fFPS;           // 当前引擎的FPS
    bool m_bCPUPerf;        // 是否启用CPU高精度计时器

public:
    Timer(void);
    ~Timer(void);

    /** 更新引擎的时钟
    @remarks
        此函数应在游戏循环的每一帧被调用
    */
    bool Update(void);

    // 获取本帧所消耗的时间
    float GetElapsed(void) const { return m_fElapsed; }

    // 获取当前的FPS
    float GetFPS(void) const { return m_fFPS; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __TIMER_H__