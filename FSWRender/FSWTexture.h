/*
 * -----------------------------------------------------------------------
 *  Name:   FSWTexture.h
 *  Desc:   本文件定义了一个纹理对象。
 *  Author: Yish
 *  Date:   2011/7/28
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FSWTEXTURE_H__
#define __FSWTEXTURE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FSWSurface.h"

//////////////////////////////////////////////////////////////////////////


/** 纹理对象
*/
class FSWTexture
{
private:
    bool m_bAlpha;              // 是否半透明
    FSWSurface* m_pSurface;     // 存储像素数据的指针

public:
    FSWTexture(void);
    ~FSWTexture(void);

    // 创建纹理对象
    bool Create( UINT nWidth, UINT nHeight );

    // 从位图中加载
    bool LoadFromBitmap( const char* filename );

    // 保存数据到位图
    void SaveToBitmap( const char* filename );

    // 获取纹理数据
	DWORD* GetBufferData(void) { return m_pSurface ? (DWORD*)m_pSurface->GetBufferData() : NULL; }

    // 获取纹理的宽度和高度
	UINT GetWidth(void) const { return m_pSurface ? m_pSurface->GetWidth() : 0; }
	UINT GetHeight(void) const { return m_pSurface ? m_pSurface->GetHeight() : 0; }

    // 获取纹理的Surface数据
    FSWSurface* _GetSurface() const { return m_pSurface; }

    // 所加载的纹理是否含有Alpha信息
    bool HaveAlpha() const { return m_bAlpha; }

    // 保存指定的渲染表面
    static void SaveSurfaceToFile( FSWSurface* pSurface, const char* filename );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWTEXTURE_H__