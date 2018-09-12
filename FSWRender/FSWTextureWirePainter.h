/*
 * ------------------------------------------------------------------------
 *  Name:   FSWTextureWirePainter.h
 *  Desc:   本文件定义了一个带纹理的高罗德着色器（线框模式）。
 *  Author: Yish
 *  Date:   2013/3/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FSW_TEXTUREWIREPAINTER_H__
#define __FSW_TEXTUREWIREPAINTER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FSWPainter.h"

///////////////////////////////////////////////////////////////////////////


class FSWTextureWirePainter : public FSWPainter
{
public:
    FSWTextureWirePainter( SRenderState* pRS );
    ~FSWTextureWirePainter();

    // 绘制三角形列表
    void DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
        UINT nTriangleCount );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSW_TEXTUREWIREPAINTER_H__