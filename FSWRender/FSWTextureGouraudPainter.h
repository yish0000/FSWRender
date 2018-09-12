/*
 * -----------------------------------------------------------------------
 *  Name:   FSWTextureGouraudPainter.h
 *  Desc:   ���ļ�������һ��������ĸ��޵���ɫ����
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


/** ������ĸ��޵���ɫ��
*/
class FSWTextureGouraudPainter : public FSWPainter
{
public:
    FSWTextureGouraudPainter( SRenderState* pRS );
    ~FSWTextureGouraudPainter();

    // �����������б�
    void DrawTriangleList( SVertex2D* pVerts,UINT nVertCount,const WORD* pIndis,
        UINT nTriangleCount );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FSWTEXTUREGOURAUDPAINTER_H__