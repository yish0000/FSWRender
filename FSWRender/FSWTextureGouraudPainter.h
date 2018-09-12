/*
 * -----------------------------------------------------------------------
 *  Name:   FSWTextureGouraudPainter.h
 *  Desc:   本文件定义了一个带纹理的高罗德着色器。
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FSWTEXTUREGOURAUDPAINTER_H__
#define __FSWTEXTUREGOURAUDPAINTER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FSWPainter.h"

//////////////////////////////////////////////////////////////////////////


/** 带纹理的高罗德着色器
*/
class FSWTextureGouraudPainter : public FSWPainter
{
public:
    FSWTextureGouraudPainter( SRenderState* pRS );
    ~FSWTextureGouraudPainter();

    // 绘制三角形列表
    void DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
        UINT nTriangleCount );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWTEXTUREGOURAUDPAINTER_H__