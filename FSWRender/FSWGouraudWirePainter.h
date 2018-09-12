/*
 * -----------------------------------------------------------------------
 *  Name:   FSWGouraudWirePainter.h
 *  Desc:   本文件定义了一个高罗德着色器(线框模式)。
 *  Author: Yish
 *  Date:   2011/7/24
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FSWGOURAUDWIREPAINTER_H__
#define __FSWGOURAUDWIREPAINTER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FSWPainter.h"

//////////////////////////////////////////////////////////////////////////


/** 高罗德线框着色器
*/
class FSWGouraudWirePainter : public FSWPainter
{
public:
    FSWGouraudWirePainter( SRenderState* pRS );
    ~FSWGouraudWirePainter();

    // 绘制三角形列表
    void DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,UINT nTriangleCount );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWGOURAUDWIREPAINTER_H__