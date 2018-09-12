/*
 * -----------------------------------------------------------------------
 *  Name:   FSWGouraudPainter.h
 *  Desc:   本文件定义了一个高罗德着色器。
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FSWGOURAUDPAINTER_H__
#define __FSWGOURAUDPAINTER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FSWPainter.h"

//////////////////////////////////////////////////////////////////////////


/** 高罗德着色器
*/
class FSWGouraudPainter : public FSWPainter
{
public:
    FSWGouraudPainter( SRenderState* pRS );
    ~FSWGouraudPainter();

    // 绘制三角形列表
    void DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
        UINT nTriangleCount );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWGOURAUDPAINTER_H__