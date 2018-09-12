/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPostEffect.h
 *  Desc:   本文件定义了一个后期处理效果基类。
 *  Author: Yish
 *  Date:   2013/3/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_POSTEFFECT_H__
#define __FSW_POSTEFFECT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "Common.h"
#include "FSWSurface.h"

///////////////////////////////////////////////////////////////////////////


enum EPostEffectType
{
    POST_BW,                // 黑白效果
    POST_INVERT,            // 反转效果
    POST_BLOOM,             // 柔光效果
    POST_MOTIONBLUR,        // 运动模糊
    POST_OLDTV,             // 老旧电视效果
    POST_OLDMOVIE,          // 老旧电影效果
    POST_GLASS,             // 玻璃滤镜效果
    POST_CARTOON,           // 卡通效果
    POST_ASCII,             // 字符图形效果
    POST_HDR,               // 高动态光照效果
    POST_TILING,            // 块状渲染效果
    POST_NIGHTVISION,       // 夜视效果
    POST_RADIALBLUR,        // 径向模糊
    POST_EMBOSSED,          // 浮雕效果
    POST_DITHER,            // 黑白点效果
    POST_HALFTONE,          // 黑白块效果
};

/** 后期处理特效基类
*/
class FSWPostEffect
{
public:
    FSWPostEffect( EPostEffectType Type, FSWSurface* pSurface );
    virtual ~FSWPostEffect();

    // 应用后期处理特效
    virtual void ApplyEffect() = 0;

    FSWSurface* GetPrevSurface() { return m_pPrevSurface; }
    FSWSurface* GetNextSurface() { return m_pNextSurface; }

    // 设置前一个图形缓冲
    void SetPrevSurface( FSWSurface* pSurface ) { m_pPrevSurface = pSurface; }

    // 获取后期特效的类型
    EPostEffectType GetEffectType() const { return m_Type; }

	// 重新创建surface
	void OnResetDevice();

	// 创建指定类型的后期特效
	static FSWPostEffect* CreatePostEffect(EPostEffectType Type, FSWSurface* pSurface);

protected:
    EPostEffectType m_Type;         // 后期特效的类型
    FSWSurface* m_pPrevSurface;     // 上一个图形缓冲
    FSWSurface* m_pNextSurface;     // 下一个图形缓冲
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSW_POSTEFFECT_H__