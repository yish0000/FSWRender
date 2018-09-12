/*
 * ------------------------------------------------------------------------
 *  Name:   FSWPainter.h
 *  Desc:   本文件定义了一个图形绘制器基类。
 *  Author: Yish
 *  Date:   2011/7/24
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSWPAINTER_H__
#define __FSWPAINTER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWTypes.h"
#include "FSWSurface.h"
#include "FSWTexture.h"

///////////////////////////////////////////////////////////////////////////


enum EPainterType
{
    PAINT_GOURAUD,          // 高罗德着色
    PAINT_GOURAUD_WIRE,     // 高罗德着色（线框）
    PAINT_TEXTURE,          // 带纹理的高罗德着色
    PAINT_TEXTURE_WIRE,     // 带纹理的高罗德着色（线框）

    PAINT_NUM,
};

struct SRenderState
{
    FSWSurface* pBackBuffer;        // 后台缓冲区指针
    FSWSurface* pZBuffer;           // 深度缓冲区指针
    UINT nWidth, nHeight;           // 后台缓冲区的宽高
    RECT rcViewport;                // 窗口裁剪矩形
    FSWTexture* pTexture;           // 纹理指针
    EShadeMode ShadeMode;           // 着色模式
    ECullMode CullMode;             // 背面剪裁模式
    EFillMode FillMode;             // 设备的三角形填充模式
    ETextureFilter TexFilter;       // 纹理过滤模式
    bool bAlphaBlend;               // 是否开启Alpha混合
};

/** 图形绘制器基类
*/
class FSWPainter
{
public:
    FSWPainter( EPainterType Type, SRenderState* pRS ) : m_Type(Type), m_pRS(pRS) {}

    // 绘制三角形列表
    virtual void DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
        UINT nTriangleCount ) = 0;

    // 获取绘制器的类型
    EPainterType GetType() const { return m_Type; }

    // 获取设备渲染状态
    SRenderState* GetRenderState() { return m_pRS; }

    // 创建指定的绘制器
    static FSWPainter* CreatePainter( EPainterType Type, SRenderState* pRS );

protected:
    EPainterType m_Type;        // 着色器的类型
    SRenderState* m_pRS;        // 当前设备的渲染状态
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWPAINTER_H__