/*
 * ------------------------------------------------------------------------
 *  Name:   FSWSurface.h
 *  Desc:   本文件定义了一个图形表面对象。
 *  Author: Yish
 *  Date:   2011/7/20
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSWSURFACE_H__
#define __FSWSURFACE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 图形表面对象
*/
class FSWSurface
{
private:
    void* m_pData;          // 图形表面的数据
    EPixelFormat m_Format;  // 像素的格式
    UINT m_nWidth;          // 图形表面的宽度
    UINT m_nHeight;         // 图形表面的高度

public:
    FSWSurface(void);
    ~FSWSurface(void);

    // 创建缓冲区
    bool Create( UINT nWidth,UINT nHeight,EPixelFormat format );

    // 获取缓冲区数据指针
    void* GetBufferData(void) { return m_pData; }

    // 获取宽度和高度
    UINT GetWidth(void) const { return m_nWidth; }
    UINT GetHeight(void) const { return m_nHeight; }

    // 获取缓冲区的像素格式
    EPixelFormat GetPixelFormat(void) const { return m_Format; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWSURFACE_H__